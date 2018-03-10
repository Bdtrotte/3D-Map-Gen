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
        mesh += makeSide(p, tile.topHeight());
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

// TODO this needs to do a bunch more
PartialMeshData AbstractPolygonTileMesher::makeSide(const BetterPolygon &polygon, float height) const
{
    PartialMeshData mesh;

    for (int i = 0; i < polygon.points().size(); ++i) {
        int j = (i + 1) % polygon.points().size();

        QPointF a = polygon.points()[i];
        QPointF b = polygon.points()[j];
        QPointF xzCenter = (a + b) / 2;

        float lowerHeight = 0;

        QVector2D dir(b - a);
        QVector2D normal(-dir.y(), dir.x());
        normal.normalize();
        QVector3D center(xzCenter.x(), (height + lowerHeight) / 2, xzCenter.y());

        float h = height - lowerHeight;
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
