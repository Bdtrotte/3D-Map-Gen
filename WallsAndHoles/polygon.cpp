#include "polygon.h"

#include "geometry.h"

Polygon::Polygon(const QVector<QPointF> &points)
{

}

bool Polygon::chordIsClear(int ind1, int ind2) const
{
    if (ind2 == ((ind1 + 1) % mPoints.size())
            || ind1 == ((ind2 + 1) % mPoints.size()))
        return false;

    QLineF line(mPoints[ind1], mPoints[ind2]);

    if (!contains(line.center())) return false;

    for (int i = 0; i < mPoints.size(); ++i) {
        int j = (i + 1) % mPoints.size();

        if ((j == ind1 && i == ind2) || (i == ind1 && j == ind2)) continue;

        if (line.intersect(QLineF(mPoints[i], mPoints[j]), nullptr) == QLineF::BoundedIntersection)
            return false;
    }

    return true;
}

QPair<Polygon, Polygon> Polygon::splitPolygon() const
{
    //go through all chords, and choose the first that is clear

    for (int i = 0; i < mPoints.size(); ++i) {
        for (int j = i + 2; j < mPoints.size(); ++j) {
            if (chordIsClear(i, j)) {
                QVector<QPointF> p1, p2;

                int progress = i;
                while (true) {
                    if (progress == j) {
                        p1.append(mPoints[progress]);
                        break;
                    }

                    p1.append(mPoints[progress]);
                    progress = (progress + 1) % mPoints.size();
                }
                while (true) {
                    if (progress == i) {
                        p2.append(mPoints[progress]);
                        break;
                    }

                    p2.append(mPoints[progress]);
                    progress = (progress + 1) % mPoints.size();
                }

                return QPair<Polygon, Polygon>(p1, p2);
            }
        }
    }

    return QPair<Polygon, Polygon>(*this, QPolygon());
}

QPair<Polygon, Polygon> Polygon::splitPolygon(QPair<int, int> chord) const
{
    int i = chord.first, j = chord.second;

    if (!chordIsClear(i, j))
        return QPair<Polygon, Polygon>(*this, QPolygon());

    QVector<QPointF> p1, p2;

    int progress = i;
    while (true) {
        if (progress == j) {
            p1.append(mPoints[progress]);
            break;
        }

        p1.append(mPoints[progress]);
        progress = (progress + 1) % mPoints.size();
    }
    while (true) {
        if (progress == i) {
            p2.append(mPoints[progress]);
            break;
        }

        p2.append(mPoints[progress]);
        progress = (progress + 1) % mPoints.size();
    }

    return QPair<Polygon, Polygon>(p1, p2);
}

QList<Triplet<QPointF, QPointF, QPointF>> Polygon::triangulate() const
{
    if (mPoints.size() == 3) {
        if (Geometry::orientation(mPoints[0], mPoints[1], mPoints[2]) == 2)
            return { {mPoints[0], mPoints[1], mPoints[2]} };
        else
            return { {mPoints[0], mPoints[2], mPoints[1]} };
    }

    QPair<Polygon, Polygon> split = splitPolygon();
    QList<Triplet<QPointF, QPointF, QPointF>> trigs = split.first.triangulate();
    trigs.append(split.second.triangulate());

    return trigs;
}

bool Polygon::contains(const QPointF &point) const
{
    QPointF outPoint = QPointF(point.x(), boundingRect().top() + 1);

    //odd even fill
    return (allIntersections(QLineF(outPoint, point)).size() % 2) == 1;
}

QRectF Polygon::boundingRect() const
{
    if (mPoints.isEmpty()) return QRectF();

    QPointF topLeft, bottomRight;
    topLeft = bottomRight = mPoints.first();

    for (auto i = mPoints.begin() + 1; i != mPoints.end(); ++i) {
        const QPointF &p = *i;

        if (p.x() < topLeft.x()) topLeft.setX(p.x());
        else if (p.x() > bottomRight.x()) bottomRight.setX(p.x());

        if (p.y() < topLeft.y()) topLeft.setY(p.y());
        else if (p.y() > bottomRight.y()) bottomRight.setY(p.y());
    }

    return QRectF(topLeft, bottomRight);
}

QPair<QPointF, QPair<int, int>> Polygon::firstIntersectionPoint(const Polygon &other, int startingPoint, int direction) const
{
    int end, inc;
    if (direction == 1) {
        end = (startingPoint - 1 + mPoints.size()) % mPoints.size();
        inc = 1;
    } else if (direction == 2) {
        end = (startingPoint + 1) % mPoints.size();
        inc = -1;
    } else {
        Q_ASSERT(false);
    }

    for (int i = 0; i != end;) {
        int j = (i + inc + mPoints.size()) % mPoints.size();

        QLineF line(mPoints[i], mPoints[j]);

        for (int a = 0; a < other.points().size(); ++a) {
            int b = (a + 1) % other.points().size();

            QPointF intPoint;
            if (line.intersect(QLineF(other[a], other[b]), &intPoint) == QLineF::BoundedIntersection)
                return QPair<QPointF, QPair<int, int>>(intPoint, QPair<int, int>(i, j));
        }

        i = j;
    }

    return QPair<QPointF, QPair<int, int>>(QPointF(), QPair<int, int>(-1, -1));
}

QVector<QPointF> Polygon::allIntersections(const QLineF &line) const
{
    if (mPoints.isEmpty()) return {};

    QVector<QPointF> points;

    for (int i = 0; i <= mPoints.size(); ++i) {
        int j = (i + 1) % mPoints.size();
        QPointF p;
        if (line.intersect(QLineF(mPoints[i], mPoints[j]), &p) == QLineF::BoundedIntersection)
            points.append(p);
    }

    return points;
}

QVector<Polygon> Polygon::subtracted(const Polygon &other) const
{
    if (other.points().isEmpty()) return { *this };

    bool firstPointContained = contains(other[0]);
    bool noOverlap = true;

    for (const QPointF &p : other.points()) {
        if (contains(p) != firstPointContained) {
            noOverlap = false;
            break;
        }
    }

    //The cases that the polygons go not intersect at all.
    if (noOverlap && !firstPointContained) {
        return {};
    }
}

void Polygon::insertPointOnEdge(const QPointF &point, int before)
{
    mPoints.insert(before + 1, point);
}
