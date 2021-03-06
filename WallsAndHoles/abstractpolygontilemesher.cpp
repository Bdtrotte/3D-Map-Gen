#include "abstractpolygontilemesher.h"

using namespace M2M;

AbstractPolygonTileMesher::AbstractPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractTileMesher(nbhd) {}

QVector<QSharedPointer<SimpleTexturedObject>> AbstractPolygonTileMesher::makeMesh(QVector2D offset)
{
    QVector<QPointF> tp = {
        QPointF(offset.x()    , offset.y()),
        QPointF(offset.x()    , offset.y() + 1),
        QPointF(offset.x() + 1, offset.y() + 1),
        QPointF(offset.x() + 1, offset.y())
    };

    QVector<const Tile *> heightAndMaterialInfo;

    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> topPolys = topPolygons(&heightAndMaterialInfo);
    for (Triplet<BetterPolygon, QVector<float>, QVector<bool>> &p : topPolys)
        p.getFirst().translate(offset.toPointF());

    BetterPolygon groundBase(tp);
    QVector<BetterPolygon> ground;
    if (topPolys.isEmpty()) {
        ground.append(groundBase);
    } else {
        ground = groundBase.subtract(topPolys.first().getFirst());
        for (auto i = topPolys.cbegin() + 1; i != topPolys.cend(); ++i) {
            QVector<BetterPolygon> tmp;
            for (const BetterPolygon &p : ground)
                tmp.append(p.subtract(i->getFirst()));

            ground.swap(tmp);
        }
    }

    const Tile *tile = mTileNeighborhood.centerTile();

    PartialMeshData mesh;
    for (const BetterPolygon &p : ground)
        mesh += makeTop(p, 0, TileMaterial::getDefaultGroundMaterial());
    for (int i = 0; i < topPolys.size(); ++i) {
        const Triplet<BetterPolygon, QVector<float>, QVector<bool>> &p = topPolys[i];
        if (!heightAndMaterialInfo.isEmpty())
            tile = heightAndMaterialInfo[i];

        mesh += makeTop(p.getFirst(), tile->height(), tile->topMaterial());
        mesh += makeSide(p.getFirst(), tile->height(), p.getSecond(), p.getThird(), tile->sideMaterial());
    }

    return mesh.constructObjects();
}

PartialMeshData AbstractPolygonTileMesher::makeTop(const BetterPolygon &polygon,
                                                   float height,
                                                   const TileMaterial *material) const
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

        mesh.addTrig(M2M::Trig(material,
                               material,
                               v1, t1,
                               v2, t2,
                               v3, t3));
    }

    return mesh;
}

PartialMeshData AbstractPolygonTileMesher::makeSide(const BetterPolygon &polygon,
                                                    float startHegiht,
                                                    const QVector<float> endHeight,
                                                    const QVector<bool> dropWall,
                                                    const TileMaterial *material) const
{
    PartialMeshData mesh;

    for (int i = 0; i < polygon.points().size(); ++i) {
        if (!dropWall[i]) continue;
        int j = (i + 1) % polygon.points().size();

        QPointF a = polygon.points()[i];
        QPointF b = polygon.points()[j];

        float otherHeight = endHeight[i];

        QPointF xzCenter = (a + b) / 2;

        QVector2D dir(b - a);
        QVector2D normal(-dir.y(), dir.x());
        normal.normalize();
        QVector3D center(xzCenter.x(), (startHegiht + otherHeight) / 2, xzCenter.y());

        float h = startHegiht - otherHeight;
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
                                                 material,
                                                 material,
                                                 upsideDown));
    }

    return mesh;
}
