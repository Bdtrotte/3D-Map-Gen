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

Triplet<BetterPolygon, QVector<float>, QVector<bool>> makeDiagonal(QPointF meA, QPointF meB, QPointF otherA, QPointF otherB)
{
    QVector<QPointF> points = {
        meA,
        meB,
        lineHitsEdgeAt(QLineF(meB, otherA)),
        lineHitsEdgeAt(QLineF(meA, otherB))
    };

    return {points, QVector<float>(4, 0), {false, true, false, true}};
}

QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> BlockyPolygonTileMesher::topPolygons()
{
    const Tile *me = mTileNeighborhood.centerTile();

    if (me == nullptr
            || !me->hasTileTemplate())
        return {};

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
    //Go through each side manually, and check if a diagonal should be made. This should be changed in the future
    //to be one funtion which can be use on every side. For now:

    //NORTH :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if (topWallDrops[0]) {
        //This will be false if a bridge has already been made

        const Tile *far = mTileNeighborhood(0, -2);
        const Tile *left = mTileNeighborhood(-1, -1);
        const Tile *right = mTileNeighborhood(1, -1);

        QPointF meA = center + QPointF(-halfThickness, -halfThickness);
        QPointF meB = center + QPointF(halfThickness, -halfThickness);

        int dia = shouldDiagonal(me, far, left, right);
        if (dia) {
            topWallDrops[0] = false;

            QPointF meA = center + QPointF(halfThickness, -halfThickness);
            QPointF meB = center + QPointF(halfThickness, halfThickness);

            switch(dia) {
            case 1: {
                QPointF otherA = QPointF(-1, -1) + left->position().toPointF() + QPointF(left->thickness() / 2, -left->thickness() / 2);
                QPointF otherB = QPointF(-1, -1) + left->position().toPointF() + QPointF(left->thickness() / 2, left->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            case 2: {
                QPointF otherA = QPointF(1, -1) + right->position().toPointF() + QPointF(-right->thickness() / 2, right->thickness() / 2);
                QPointF otherB = QPointF(1, -1) + right->position().toPointF() + QPointF(-right->thickness() / 2, -right->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            }
        }
    }


    //WEST :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if (topWallDrops[1]) {
        //This will be false if a bridge has already been made

        const Tile *far = mTileNeighborhood(-2, 0);
        const Tile *left = mTileNeighborhood(-1, 1);
        const Tile *right = mTileNeighborhood(-1, -1);

        int dia = shouldDiagonal(me, far, left, right);
        if (dia) {
            topWallDrops[1] = false;

            QPointF meA = center + QPointF(halfThickness, -halfThickness);
            QPointF meB = center + QPointF(halfThickness, halfThickness);

            switch(dia) {
            case 1: {
                QPointF otherA = QPointF(-1, 1) + left->position().toPointF() + QPointF(-left->thickness() / 2, -left->thickness() / 2);
                QPointF otherB = QPointF(-1, 1) + left->position().toPointF() + QPointF(left->thickness() / 2, -left->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            case 2: {
                QPointF otherA = QPointF(-1, -1) + right->position().toPointF() + QPointF(right->thickness() / 2, right->thickness() / 2);
                QPointF otherB = QPointF(-1, -1) + right->position().toPointF() + QPointF(-right->thickness() / 2, right->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            }
        }
    }


    //SOUTH :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if (topWallDrops[2]) {
        //This will be false if a bridge has already been made

        const Tile *far = mTileNeighborhood(0, 2);
        const Tile *left = mTileNeighborhood(1, 1);
        const Tile *right = mTileNeighborhood(-1, 1);

        int dia = shouldDiagonal(me, far, left, right);
        if (dia) {
            topWallDrops[2] = false;

            QPointF meA = center + QPointF(halfThickness, -halfThickness);
            QPointF meB = center + QPointF(halfThickness, halfThickness);

            switch(dia) {
            case 1: {
                QPointF otherA = QPointF(1, 1) + left->position().toPointF() + QPointF(-left->thickness() / 2, left->thickness() / 2);
                QPointF otherB = QPointF(1, 1) + left->position().toPointF() + QPointF(-left->thickness() / 2, -left->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            case 2: {
                QPointF otherA = QPointF(-1, 1) + right->position().toPointF() + QPointF(right->thickness() / 2, -right->thickness() / 2);
                QPointF otherB = QPointF(-1, 1) + right->position().toPointF() + QPointF(right->thickness() / 2, right->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            }
        }
    }


    //EAST :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    if (topWallDrops[3]) {
        //This will be false if a bridge has already been made

        const Tile *far = mTileNeighborhood(0, -2);
        const Tile *left = mTileNeighborhood(1, -1);
        const Tile *right = mTileNeighborhood(1, 1);

        int dia = shouldDiagonal(me, far, left, right);
        if (dia) {
            topWallDrops[3] = false;

            QPointF meA = center + QPointF(halfThickness, -halfThickness);
            QPointF meB = center + QPointF(halfThickness, halfThickness);

            switch(dia) {
            case 1: {
                QPointF otherA = QPointF(1, -1) + left->position().toPointF() + QPointF(left->thickness() / 2, left->thickness() / 2);
                QPointF otherB = QPointF(1, -1) + left->position().toPointF() + QPointF(-left->thickness() / 2, left->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
            case 2: {
                QPointF otherA = QPointF(1, 1) + right->position().toPointF() + QPointF(-right->thickness() / 2, -right->thickness() / 2);
                QPointF otherB = QPointF(1, 1) + right->position().toPointF() + QPointF(right->thickness() / 2, -right->thickness() / 2);

                ret.append(makeDiagonal(meA, meB, otherA, otherB));
            } break;
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
