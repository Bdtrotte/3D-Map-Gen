#include "blockypolygontilemesher.h"

using namespace M2M;

BlockyPolygonTileMesher::BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractPolygonTileMesher(nbhd)
{
    //THIS MESHER SHOULD NOT BE GIVEN TO GROUND TILES
    Q_ASSERT(mTileNeighborhood.centerTile()->hasTileTemplate());
}

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

//0 NO
//1 Left
//2 Right
int shouldDiagonal(const Tile *center, const Tile *far, const Tile *left, const Tile *right)
{
    if (far == nullptr || far->tileTemplate() != center->tileTemplate()) {
        //If the tile two cells in front of this is on the same template type, a diagonal wont be made

        if (left != nullptr
                && left->tileTemplate() == center->tileTemplate()
                && (right == nullptr || right->tileTemplate() != center->tileTemplate()))
            return 1;

        if (right != nullptr
                && right->tileTemplate() == center->tileTemplate()
                && (left == nullptr || left->tileTemplate() != center->tileTemplate()))
            return 2;
    }

    return 0;
}

//edge refers to the edge of a unit box.
QPointF lineHitsEdgeAt(const QLineF &line)
{
    QPointF ret;

    if (line.intersect(QLineF(0, 0, 1, 0), &ret) == QLineF::BoundedIntersection)
        return ret;

    if (line.intersect(QLineF(1, 0, 1, 1), &ret) == QLineF::BoundedIntersection)
        return ret;

    if (line.intersect(QLineF(1, 1, 0, 1), &ret) == QLineF::BoundedIntersection)
        return ret;

    if (line.intersect(QLineF(0, 1, 0, 0), &ret) == QLineF::BoundedIntersection)
        return ret;

    return line.p2();
}

Triplet<BetterPolygon, QVector<float>, QVector<bool>> makeDiagonal(QPointF meA, QPointF meB, QPointF otherA, QPointF otherB, QVector<bool> dropWalls)
{
    QVector<QPointF> points = {
        meA,
        meB,
        lineHitsEdgeAt(QLineF(meB, otherA)),
        lineHitsEdgeAt(QLineF(meA, otherB))
    };

    return {points, QVector<float>(4, 0), dropWalls};

}

QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> BlockyPolygonTileMesher::topPolygons(QVector<const Tile *> *)
{
    const Tile *me = mTileNeighborhood.centerTile();

    QPointF center = me->position().toPointF();
    float halfThickness = me->thickness() / 2;

    QVector<QPointF> topPoints = {
        center + QPointF(halfThickness, -halfThickness),
        center + QPointF(-halfThickness, -halfThickness),
        center + QPointF(-halfThickness, halfThickness),
        center + QPointF(halfThickness, halfThickness)
    };

    QVector<bool> topWallDrops(4, true);

    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> ret;

    if (halfThickness < 0.5) {

//BRIDGING======================================================================================================

        //North
        if (mTileNeighborhood(0, -1) != nullptr
                && me->tileTemplate() == mTileNeighborhood(0, -1)->tileTemplate()) {
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
                && me->tileTemplate() == mTileNeighborhood(1, 0)->tileTemplate()) {
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
                && me->tileTemplate() == mTileNeighborhood(0, 1)->tileTemplate()) {
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
                && me->tileTemplate() == mTileNeighborhood(-1, 0)->tileTemplate()) {
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

//DIAGONALS======================================================================================================
    //Start by gathering how each side should "diagonal" (left right, or not at all)
    const Tile *far[4];
    const Tile *left[4];
    const Tile *right[4];
    int shouldDia[4];

    //NORTH
    far[0] = mTileNeighborhood(0, -2);
    left[0] = mTileNeighborhood(-1, -1);
    right[0] = mTileNeighborhood(1, -1);
    shouldDia[0] = topWallDrops[0]? shouldDiagonal(me, far[0], left[0], right[0]) : 0;

    //WEST
    far[1] = mTileNeighborhood(-2, 0);
    left[1] = mTileNeighborhood(-1, 1);
    right[1] = mTileNeighborhood(-1, -1);
    shouldDia[1] = topWallDrops[1]? shouldDiagonal(me, far[1], left[1], right[1]) : 0;

    //SOUTH
    far[2] = mTileNeighborhood(0, 2);
    left[2] = mTileNeighborhood(1, 1);
    right[2] = mTileNeighborhood(-1, 1);
    shouldDia[2] = topWallDrops[2]? shouldDiagonal(me, far[2], left[2], right[2]) : 0;

    //EAST
    far[3] = mTileNeighborhood(2, 0);
    left[3] = mTileNeighborhood(1, -1);
    right[3] = mTileNeighborhood(1, 1);
    shouldDia[3] = topWallDrops[3]? shouldDiagonal(me, far[3], left[3], right[3]) : 0;

    QPoint corners[4] = {
        QPoint(-1, -1),
        QPoint(-1, 1),
        QPoint(1, 1),
        QPoint(1, -1)
    };

    //Now go through and use the info to buid diagonals

    for (int i = 0; i < 4; ++i) {
        if (shouldDia[i]) {
            topWallDrops[i] = false;

            if (halfThickness < 0.5) {
                QPointF meA = center + QPointF(corners[i].x() * halfThickness, corners[i].y() * halfThickness);
                QPointF meB = center + QPointF(corners[(i + 3) % 4].x() * halfThickness, corners[(i + 3) % 4].y() * halfThickness);

                switch(shouldDia[i]) {
                case 1: {
                    QPointF otherA = corners[i] + left[i]->position().toPointF() + QPointF(corners[(i + 3) % 4].x() * left[i]->thickness() / 2, corners[(i + 3) % 4].y() * left[i]->thickness() / 2);
                    QPointF otherB = corners[i] + left[i]->position().toPointF() + QPointF(corners[(i + 2) % 4].x() * left[i]->thickness() / 2, corners[(i + 2) % 4].y() * left[i]->thickness() / 2);

                    ret.append(makeDiagonal(meA, meB, otherA, otherB, {false, true, false, shouldDia[(i + 1) % 4] != 2}));
                } break;
                case 2: {
                    QPointF otherA = corners[(i + 3) % 4] + right[i]->position().toPointF() + QPointF(corners[(i + 1) % 4].x() * right[i]->thickness() / 2, corners[(i + 1) % 4].y() * right[i]->thickness() / 2);
                    QPointF otherB = corners[(i + 3) % 4] + right[i]->position().toPointF() + QPointF(corners[i].x() * right[i]->thickness() / 2, corners[i].y() * right[i]->thickness() / 2);

                    ret.append(makeDiagonal(meA, meB, otherA, otherB, {false, shouldDia[(i + 3) % 4] != 1, false, true}));
                } break;
                }
            }
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
