#include "blockypolygontilemesher.h"

using namespace M2M;

BlockyPolygonTileMesher::BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractPolygonTileMesher(nbhd) {}

void BlockyPolygonTileMesher::determinIfWallShouldDrop(QPointF a, QPointF b, float &height, bool &shouldDrop)
{
    //base values ensured.
    height = 0;
    shouldDrop = true;

    //First see if these points are on a common tile edge;
    //-1 None
    //0 North
    //1 West
    //2 South
    //3 East
    int edge = -1;
    if (a.x() == b.x()) {
        if (a.x() == 0)
            edge = 1;
        else if (a.x() == 1)
            edge = 3;
    } else if (a.y() == a.y()) {
        if (a.y() == 0)
            edge = 0;
        else if (a.y() == 1)
            edge = 2;
    }

    if (edge == -1) return;

    const Tile *other;
    switch (edge) {
    case 0: //NORTH
        other = mTileNeighborhood(0, -1);
        break;
    case 1: //WEST
        other = mTileNeighborhood(-1, 0);
        break;
    case 2: //SOUTH
        other = mTileNeighborhood(0, 1);
        break;
    case 3: //EAST
        other = mTileNeighborhood(1, 0);
        break;
    }

    if (mTileNeighborhood.centerTile()->thickness() == 1) {
        if (other == nullptr) {
            shouldDrop = false;
            return;
        }
        if (!other->hasTileTemplate())
            return;

        if (other->thickness() == 1) {
            if (other->height() >= mTileNeighborhood.centerTile()->height()) {
                shouldDrop = false;
                return;
            }

            height = other->height();
        }
    }
}

QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> BlockyPolygonTileMesher::topPolygons()
{
    if (!mTileNeighborhood.centerTile()->hasTileTemplate()) return {};

    QPointF center = mTileNeighborhood.centerTile()->position().toPointF();
    float halfThickness = mTileNeighborhood.centerTile()->thickness() / 2;

    QVector<QPointF> topPoints = {
        center + QPointF(halfThickness, -halfThickness),
        center + QPointF(-halfThickness, -halfThickness),
        center + QPointF(-halfThickness, halfThickness),
        center + QPointF(halfThickness, halfThickness)
    };

    QVector<bool> topWallDrops(4, true);

    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> ret;

    if (halfThickness < 0.5) {
        //Bridging

        //North
        if (mTileNeighborhood(0, -1) != nullptr
                && mTileNeighborhood.centerTile()->tileTemplate() == mTileNeighborhood(0, -1)->tileTemplate()) {
            QVector<QPointF> bridge = {
                center + QPointF(-halfThickness, -halfThickness),
                center + QPointF(halfThickness, -halfThickness),
                QPointF(halfThickness + center.x(), 0),
                QPointF(-halfThickness + center.x(), 0)
            };

            ret.append({bridge, QVector<float>(4, 0), {false, true, false, true}});
            topWallDrops[0] = false;
        }

        //East
        if (mTileNeighborhood(1, 0) != nullptr
                && mTileNeighborhood.centerTile()->tileTemplate() == mTileNeighborhood(1, 0)->tileTemplate()) {
            QVector<QPointF> bridge = {
                center + QPointF(halfThickness, -halfThickness),
                center + QPointF(halfThickness, halfThickness),
                QPointF(1, halfThickness + center.y()),
                QPointF(1, -halfThickness + center.y())
            };

            ret.append({bridge, QVector<float>(4, 0), {false, true, false, true}});
            topWallDrops[3] = false;
        }

        //South
        if (mTileNeighborhood(0, 1) != nullptr
                && mTileNeighborhood.centerTile()->tileTemplate() == mTileNeighborhood(0, 1)->tileTemplate()) {
            QVector<QPointF> bridge = {
                center + QPointF(halfThickness, halfThickness),
                center + QPointF(-halfThickness, halfThickness),
                QPointF(-halfThickness + center.x(), 1),
                QPointF(halfThickness + center.x(), 1)
            };

            ret.append({bridge, QVector<float>(4, 0), {false, true, false, true}});
            topWallDrops[2] = false;
        }

        //West
        if (mTileNeighborhood(-1, 0) != nullptr
                && mTileNeighborhood.centerTile()->tileTemplate() == mTileNeighborhood(-1, 0)->tileTemplate()) {
            QVector<QPointF> bridge = {
                center + QPointF(-halfThickness, halfThickness),
                center + QPointF(-halfThickness, -halfThickness),
                QPointF(0, -halfThickness + center.y()),
                QPointF(0, halfThickness + center.y())
            };

            ret.append({bridge, QVector<float>(4, 0), {false, true, false, true}});
            topWallDrops[1] = false;
        }
    }

    QVector<float> topWalEndHeight(4, 0);
    //Now check unhandled edges to see if a wall really needs to drop, and if so, to what height.
    for (int i = 0; i < 4; ++i) {
        if (!topWallDrops[i]) continue;
        int j = (i + 1) % 4;

        determinIfWallShouldDrop(topPoints[i], topPoints[j], topWalEndHeight[i], topWallDrops[i]);
    }

    ret.append({topPoints, topWalEndHeight, topWallDrops});

    return ret;
}
