#include "blockypolygontilemesher.h"

using namespace M2M;

BlockyPolygonTileMesher::BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractPolygonTileMesher(nbhd) {}

QVector<BetterPolygon> BlockyPolygonTileMesher::topPolygon()
{
    if (mTileNeighborhood.centerTile().isGround()) return {};

    QPointF center = mTileNeighborhood.centerTile().center().toPointF();
    float halfThickness = mTileNeighborhood.centerTile().thickness() / 2;

    QVector<QPointF> topPoints = {
        center + QPointF(halfThickness, halfThickness),
        center + QPointF(halfThickness, -halfThickness),
        center + QPointF(-halfThickness, -halfThickness),
        center + QPointF(-halfThickness, halfThickness)
    };

    QVector<BetterPolygon> ret;
    ret.append(topPoints);

    if (halfThickness < 0.5) {
        //Bridging

        //North
        if (mTileNeighborhood.centerTile() == mTileNeighborhood(1, 0)) {
            QVector<QPointF> bridge = {
                center + QPointF(-halfThickness, -halfThickness),
                center + QPointF(halfThickness, -halfThickness),
                center + QPointF(halfThickness, halfThickness - 1),
                center + QPointF(-halfThickness, halfThickness - 1)
            };

            ret.append(bridge);
        }

        //East
        if (mTileNeighborhood.centerTile() == mTileNeighborhood(2, 1)) {
            QVector<QPointF> bridge = {
                center + QPointF(halfThickness, -halfThickness),
                center + QPointF(halfThickness, halfThickness),
                center + QPointF(-halfThickness + 1, halfThickness),
                center + QPointF(-halfThickness + 1, -halfThickness)
            };

            ret.append(bridge);
        }

        //South
        if (mTileNeighborhood.centerTile() == mTileNeighborhood(1, 2)) {
            QVector<QPointF> bridge = {
                center + QPointF(halfThickness, halfThickness),
                center + QPointF(-halfThickness, halfThickness),
                center + QPointF(-halfThickness, -halfThickness + 1),
                center + QPointF(halfThickness, -halfThickness + 1)
            };

            ret.append(bridge);
        }

        //West
        if (mTileNeighborhood.centerTile() == mTileNeighborhood(0, 1)) {
            QVector<QPointF> bridge = {
                center + QPointF(-halfThickness, halfThickness),
                center + QPointF(-halfThickness, -halfThickness),
                center + QPointF(halfThickness - 1, -halfThickness),
                center + QPointF(halfThickness - 1, halfThickness)
            };

            ret.append(bridge);
        }
    }

    return ret;
}
