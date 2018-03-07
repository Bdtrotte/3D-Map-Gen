#include "m2mtilemesher_private.h"

#include "triplet.h"

#include <QMap>

using namespace M2M_Private;

//orientation, onSegment, and linesIntersect were adjusted from
//https://www.cdn.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

//0 - a, b, and c are colinear
//1 - Clockwise
//2 - Counter Clockwise
int orientation(QPointF a, QPointF b, QPointF c)
{
    int val = (b.y() - a.y()) * (c.x() - b.x()) - (b.x() - a.x()) * (c.y() - b.y());

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool onSegment(QPointF a1, QPointF a2, QPointF c)
{
    if (orientation(a1, a2, c) == 0
            && c.x() >= std::min(a1.x(), a2.x()) && c.x() <= std::max(a1.x(), a2.x())
            && c.y() >= std::min(a1.y(), a2.y()) && c.y() <= std::max(a1.y(), a2.y()))
        return true;

    return false;
}

bool linesIntersect(QPointF a1, QPointF a2, QPointF b1, QPointF b2)
{
    int o1 = orientation(a1, a2, b1);
    int o2 = orientation(a1, a2, b2);
    int o3 = orientation(b1, b2, a1);
    int o4 = orientation(b1, b2, a2);

    if ((o1 != o2 && o3 != o4)
            || onSegment(a1, a2, b1)
            || onSegment(a1, a2, b2)
            || onSegment(b1, b2, a1)
            || onSegment(b1, b2, a2))
        return true;

    return false;
}

/**
 * @brief chordIntersects
 *
 * Does the given chord intersect any other edge of the given polygon
 *
 * @param polygon
 * @param ind1
 * @param ind2
 * @return
 */
bool chordIntersects(const QPolygonF &polygon, int ind1, int ind2)
{
    if (std::abs(ind1 - ind2) < 2) return true;

    QPointF a1 = polygon[ind1];
    QPointF a2 = polygon[ind2];

    for (int i = 0; i < polygon.size(); ++i) {
        int j = (i + 1) % polygon.size();
        if (i == ind1 || i == ind2 || j == ind1 || j == ind2) continue;

        QPointF b1 = polygon[i];
        QPointF b2 = polygon[j];

        if (linesIntersect(a1, a2, b1, b2)) return true;
    }

    return false;
}

/**
 * @brief triangulate
 *
 * triangulates a subpolygon of a given polygon.
 * Returned triangles are the indicies of the original polygon in counterclockwise order.
 *
 * @param polygon
 * @param indicies
 * @return
 */
QList<Triplet<QPointF, QPointF, QPointF>> triangulate(QPolygonF polygon)
{
    //General strategy: Go through all chords until one is found that doesn't intersect pat of the polygon,
    //then run this on the two polygons produced by splitting at the chord.

    if (polygon.isClosed()) polygon.removeLast();

    if (polygon.size() == 3)
        return { Triplet<QPointF, QPointF, QPointF>(polygon[0], polygon[1], polygon[2]) };

    int c1, c2;

    for (int i = 0; i < polygon.size(); ++i) {
        for (int j = i + 2; j < polygon.size(); ++j) {
            if (i == 0 && j == polygon.size() - 1) continue;
            QPointF mid = (polygon[i] + polygon[j])/2;
            if (polygon.containsPoint(mid, Qt::OddEvenFill) && !chordIntersects(polygon, i, j)) {
                c1 = i;
                c2 = j;
                break;
            }
        }
    }

    QPolygonF p1, p2;

    int progress = c1;
    while(true) {
        if (progress == c2) break;
        p1.append(polygon[progress]);
        progress = (progress + 1) % polygon.size();
    }
    while (true) {
        if (progress == c1) break;
        p2.append(polygon[progress]);
        progress = (progress + 1) % polygon.size();
    }

    QList<Triplet<QPointF, QPointF, QPointF>> ret = triangulate(p1);
    ret.append(triangulate(p2));

    return ret;
}

bool operator <(const QPointF &a, const QPointF &b) {
    if (a.x() == b.x())
        return a.y() <= b.y();
    else
        return a.x() > b.x();
}


M2M::PartialMeshData polygonMesh(const QPolygonF &polygon,
                                 float height,
                                 M2M::ImageInfo image,
                                 M2M::PhongInfo material)
{
    QMap<QPointF, QVector3D> verts;

    for (const QPointF &p : polygon) {
        if (!verts.contains(p))
            verts[p] = QVector3D(p.x(), height, p.y());
    }

    QList<Triplet<QPointF, QPointF, QPointF>> triangles = triangulate(polygon);

    M2M::PartialMeshData mesh;
    for (const Triplet<QPointF, QPointF, QPointF> &t : triangles) {
        QVector3D v1 = verts[t.getFirst()];
        QVector3D v2 = verts[t.getSecond()];
        QVector3D v3 = verts[t.getThird()];

        QVector2D t1(t.getFirst());
        QVector2D t2(t.getSecond());
        QVector2D t3(t.getThird());

        mesh.addTrig(M2M::Trig(image, material, v1, t1, v2, t2, v3, t3));
    }

    return mesh;
}

M2M::PartialMeshData polygonSidesMesh(QPolygonF polygon,
                                      QVector<bool> edgesToDrop,
                                      QVector<float> bottomHeight,
                                      float topHeight,
                                      M2M::ImageInfo image,
                                      M2M::PhongInfo material)
{
    if (!polygon.isClosed()) polygon.append(polygon.first());

    M2M::PartialMeshData mesh;

    for (int i = 0; i < polygon.size() - 1; ++i) {
        if (!edgesToDrop[i]) continue;

        QVector2D a(polygon[i]);
        QVector2D b(polygon[i + 1]);

        QVector2D xzCenter = (a + b) / 2;
        QVector3D center(xzCenter.x(),
                         (topHeight + bottomHeight[i])/2,
                         xzCenter.y());

        QVector2D normal(b.y() - a.y(), a.x() - b.x());
        normal.normalize();

        mesh.addQuad(M2M::Quad::makeVerticalQuad(center,
                                                 normal,
                                                 a.distanceToPoint(b),
                                                 topHeight - bottomHeight[i],
                                                 image,
                                                 material));
    }

    return mesh;
}


QPoint SideTools::neighborOffset(SideDirection side)
{
    switch (side) {
    case NORTH: return QPoint(0, -1);
    case EAST:  return QPoint(1, 0);
    case SOUTH: return QPoint(0, 1);
    case WEST:  return QPoint(-1, 0);
    default:
        Q_ASSERT( false ); // there should only be 4 side directions
    }
}

QVector2D SideTools::awayDirection(SideDirection side)
{
    switch (side) {
    case NORTH: return QVector2D(0, -1);
    case EAST:  return QVector2D(1, 0);
    case SOUTH: return QVector2D(0, 1);
    case WEST:  return QVector2D(-1, 0);
    default:
        Q_ASSERT( false ); // there should only be 4 side directions
    }
}

QVector3D SideTools::normalDirection(SideDirection side)
{
    QVector2D away = SideTools::awayDirection(side);

    return QVector3D(away.x(), 0, away.y());
}


M2M::Quad SideTools::makeTopQuadFragment(SideDirection side,
                                         QVector2D quadCenter,
                                         QVector2D minusQuadCenter,
                                         float minusQuadSize,
                                         M2M::ImageInfo quadImage,
                                         M2M::PhongInfo quadMaterial)
{
    // The normal direction is just the away direction.
    QVector3D awayDirection = SideTools::normalDirection(side);

    // 'awayDirection' rotated CCW in the XZ plane looking down along Y axis.
    QVector3D leftDirection(awayDirection.z(), 0, -awayDirection.x());

    QVector3D mainCenter(quadCenter.x(), 0, quadCenter.y());
    QVector3D subCenter(minusQuadCenter.x(), 0, minusQuadCenter.y());

    QVector3D v1 = mainCenter + awayDirection * 0.5 - leftDirection * 0.5;
    QVector3D v2 = mainCenter + awayDirection * 0.5 + leftDirection * 0.5;
    QVector3D v3 = subCenter + (awayDirection + leftDirection) * 0.5 * minusQuadSize;
    QVector3D v4 = subCenter + (awayDirection - leftDirection) * 0.5 * minusQuadSize;

    // Away and left in tile coordinates.
    QVector2D tileAway(-awayDirection.x(), awayDirection.z());
    QVector2D tileLeft(-tileAway.y(), tileAway.x());

    // minusQuadCenter - quadCenter in tile coordinates.
    QVector2D tileMinusOffset(quadCenter.x() - minusQuadCenter.x(), minusQuadCenter.y() - quadCenter.y());

    QVector2D t1 = QVector2D(0.5, 0.5) + (tileAway - tileLeft) * 0.5;
    QVector2D t2 = QVector2D(0.5, 0.5) + (tileAway + tileLeft) * 0.5;
    QVector2D t3 = QVector2D(0.5, 0.5) + tileMinusOffset + (tileAway + tileLeft) * 0.5 * minusQuadSize;
    QVector2D t4 = QVector2D(0.5, 0.5) + tileMinusOffset + (tileAway - tileLeft) * 0.5 * minusQuadSize;

    return M2M::Quad(QVector3D(0, 1, 0),
                     quadImage,
                     quadMaterial,
                     v1, t1,
                     v2, t2,
                     v3, t3,
                     v4, t4);
}
