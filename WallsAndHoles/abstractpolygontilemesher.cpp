#include "abstractpolygontilemesher.h"

using namespace M2M;

AbstractPolygonTileMesher::AbstractPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractTileMesher(nbhd) {}

QVector<QSharedPointer<SimpleTexturedObject>> AbstractPolygonTileMesher::makeMesh(QVector2D offset)
{
    const TileInfo &tile = mTileNeighborhood.centerTile();

    QVector<QPointF> tp = {
        QPointF(offset.x()    , offset.y()),
        QPointF(offset.x()    , offset.y() + 1),
        QPointF(offset.x() + 1, offset.y() + 1),
        QPointF(offset.x() + 1, offset.y())
    };

    BetterPolygon groundBase(tp);
    QVector<BetterPolygon> topPoly = topPolygon();
    for (BetterPolygon &p : topPoly)
        p.translate(offset.toPointF());

    QVector<BetterPolygon> toBuild;
    for (const BetterPolygon &t : topPoly)
        toBuild.append(t.intersect(groundBase));


    QVector<BetterPolygon> ground;
    if (toBuild.isEmpty()) {
        ground.append(groundBase);
    } else {
        ground = groundBase.subtract(toBuild.first());
        for (auto i = toBuild.cbegin() + 1; i != toBuild.cend(); ++i) {
            QVector<BetterPolygon> tmp;
            for (const BetterPolygon &p : ground)
                tmp.append(p.subtract(*i));

            ground.swap(tmp);
        }
    }

    PartialMeshData mesh;
    for (const BetterPolygon &p : ground)
        mesh += makeTop(p, 0, true);
    for (const BetterPolygon &p : toBuild) {
        mesh += makeTop(p, tile.topHeight());
        mesh += makeSide(p, tile.topHeight(), offset.toPointF());
    }

    return mesh.constructObjects();
}

PartialMeshData AbstractPolygonTileMesher::makeTop(const BetterPolygon &polygon, float height, bool ground) const
{
    QList<Triplet<QPointF, QPointF, QPointF>> triangles = polygon.triangulate();

    PartialMeshData mesh;
    for (const Triplet<QPointF, QPointF, QPointF> &t : triangles) {
        QVector3D v1(t.getFirst().x(), height, t.getFirst().y());
        QVector3D v2(t.getSecond().x(), height, t.getSecond().y());
        QVector3D v3(t.getThird().x(), height, t.getThird().y());

        QVector2D t1(t.getFirst());
        QVector2D t2(t.getSecond());
        QVector2D t3(t.getThird());

        ImageInfo imageInfo = ground? mTileNeighborhood.centerTile().groundInfo().groundImage
                                    : mTileNeighborhood.centerTile().topImage();

        PhongInfo phongInfo = ground? mTileNeighborhood.centerTile().groundInfo().groundMaterial
                                    : mTileNeighborhood.centerTile().topMaterial();

        mesh.addTrig(M2M::Trig(imageInfo,
                               phongInfo,
                               v1, t1,
                               v2, t2,
                               v3, t3));
    }

    return mesh;
}

//0 Not on a side
//1 North (-Y)
//2 East  (+X)
//3 South (+Y)
//4 West  (-X)
int edgeSide(QPointF a, QPointF b, const QPointF &offset)
{
    if (a.x() == b.x()) {
        if (a.x() - offset.x() == 0) {
            return 4;
        } else if (a.x() - offset.x() == 1) {
            return 2;
        }
    } else if (a.y() == b.y()) {
        if (a.y() - offset.y() == 0) {
            return 1;
        } else if (a.y() - offset.y() == 1) {
            return 3;
        }
    }

    return 0;
}

PartialMeshData AbstractPolygonTileMesher::makeSide(const BetterPolygon &polygon, float height, const QPointF &offset) const
{
    PartialMeshData mesh;

    for (int i = 0; i < polygon.points().size(); ++i) {
        int j = (i + 1) % polygon.points().size();

        QPointF a = polygon.points()[i];
        QPointF b = polygon.points()[j];

        float otherHeight = 0;
        TileInfo other;
        switch (edgeSide(a, b, offset)) {
        case 1:
            other = mTileNeighborhood(1, 0);
            break;
        case 2:
            other = mTileNeighborhood(2, 1);
            break;
        case 3:
            other = mTileNeighborhood(1, 2);
            break;
        case 4:
            other = mTileNeighborhood(0, 1);
            break;
        }

        // TODO anyalize other better to determin if wall should be dropped.
        //      currently some bad behavior can occure in edge cases
        if (!other.isNull()) {
            if (other.isGround())
                otherHeight = other.groundInfo().groundHeight;
            else
                otherHeight = other.topHeight();
        }

        QPointF xzCenter = (a + b) / 2;

        QVector2D dir(b - a);
        QVector2D normal(-dir.y(), dir.x());
        normal.normalize();
        QVector3D center(xzCenter.x(), (height + otherHeight) / 2, xzCenter.y());

        float h = height - otherHeight;
        bool upsideDown = false;
        if (h == 0) continue;
        if (h < 0) {
            h = -h;
            upsideDown = true;
        }

        mesh.addQuad(M2M::Quad::makeVerticalQuad(center,
                                                 normal,
                                                 dir.length(),
                                                 h,
                                                 mTileNeighborhood.centerTile().sideImage(),
                                                 mTileNeighborhood.centerTile().sideMaterial(),
                                                 upsideDown));
    }

    return mesh;
}
