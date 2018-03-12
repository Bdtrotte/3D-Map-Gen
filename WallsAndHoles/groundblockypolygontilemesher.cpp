#include "groundblockypolygontilemesher.h"

using namespace M2M;


GroundBlockyPolygonTileMesher::GroundBlockyPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractPolygonTileMesher(nbhd)
{
    //THIS MESHER SHOULD ONLY BE GIVEN TO GROUND TILES
    Q_ASSERT(!mTileNeighborhood.centerTile()->hasTileTemplate());
}

QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> makeDiagonal(QPointF a, QPointF b, QPointF c, QPointF d)
{
    QVector<QPointF> base;
    if (a == b) {
        base.append(a);
    } else {
        base.append(a);
        base.append(b);
    }

    if (b != c)
        base.append(c);
    if (c != d && d != a)
        base.append(d);

    BetterPolygon tile({QPointF(0, 0), QPointF(0, 1), QPointF(1, 1), QPointF(1, 0)});

    auto overLap = tile.intersect(base);
    if (overLap.size() == 0)
        return {};

    BetterPolygon poly = overLap.first();

    //We now have the polygon for the diagonal!
    //Next step is to determin which walls to drop
    //This will be all the edges NOT on the edge of the tile.

    QVector<bool> drops(poly.points().size(), true);

    for (int i = 0; i < poly.points().size(); ++i) {
        int j = (i + 1) % poly.points().size();

        const QPointF &a = poly.points()[i];
        const QPointF &b = poly.points()[j];

        if (a.x() == b.x()
                && (a.x() == 0 || a.x() == 1)) {
            drops[i] = false;
            continue;
        }
        if (a.y() == b.y()
                && (a.y() == 0 || a.y() == 1)) {
            drops[i] = false;
            continue;
        }
    }

    return {{ poly, QVector<float>(poly.points().size(), 0), drops }};
}

QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> GroundBlockyPolygonTileMesher::topPolygons(QVector<const Tile *> *heightAndMaterial)
{
    return {};

    //first is the inner diagonal, second is the outer diagonal
    QPair<bool, bool> topLeft(false, false);
    QPair<bool, bool> topRight(false, false);
    QPair<bool, bool> bottomLeft(false, false);
    QPair<bool, bool> bottomRight(false, false);

    //LEFT ::::::::::::::::::::::::::::::::::::::::::::::

    if (mTileNeighborhood(-1, 0) != nullptr
            && mTileNeighborhood(-1, 0)->hasTileTemplate()) {
        if (mTileNeighborhood(0, -1) != nullptr
                && mTileNeighborhood(0, -1)->tileTemplate() == mTileNeighborhood(-1, 0)->tileTemplate()) {
            //Left and Top have the same tile template
            //Now check if the inner diagonal should be made;

            bool botDiff = mTileNeighborhood(0, 1) == nullptr
                    || mTileNeighborhood(0, 1)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool rightDiff = mTileNeighborhood(1, 0) == nullptr
                    || mTileNeighborhood(1, 0)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();

            if (botDiff && rightDiff)
                topLeft.first = true;

            //Now see if the outer diagonal should be made;

            bool farTopDiff = mTileNeighborhood(-1, -2) == nullptr
                    || mTileNeighborhood(-1, -2)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool farLeftDiff = mTileNeighborhood(-2, -1) == nullptr
                    || mTileNeighborhood(-2, -1)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool centerClear = mTileNeighborhood(-1, -1) == nullptr
                    || !mTileNeighborhood(-1, -1)->hasTileTemplate();

            if (farTopDiff && farLeftDiff && centerClear)
                topLeft.second = true;
        }

        if (!topLeft.first && !topLeft.second
                && mTileNeighborhood(0, 1) != nullptr
                && mTileNeighborhood(0, 1)->tileTemplate() == mTileNeighborhood(-1, 0)->tileTemplate()) {
            //Left and top did not connect a diagonal
            //Try between Left and Bottom

            bool topDiff = mTileNeighborhood(0, -1) == nullptr
                    || mTileNeighborhood(0, -1)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool rightDiff = mTileNeighborhood(1, 0) == nullptr
                    || mTileNeighborhood(1, 0)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();

            if (topDiff && rightDiff)
                bottomLeft.first = true;

            //Now see if outer diagonal should be made

            bool farBottomDiff = mTileNeighborhood(-1, 2) == nullptr
                    || mTileNeighborhood(-1, 2)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool farLeftDiff = mTileNeighborhood(-2, 1) == nullptr
                    || mTileNeighborhood(-2, 1)->tileTemplate() != mTileNeighborhood(-1, 0)->tileTemplate();
            bool centerClear = mTileNeighborhood(-1, 1) == nullptr
                    || !mTileNeighborhood(-1, 1)->hasTileTemplate();

            if (farBottomDiff && farLeftDiff && centerClear)
                bottomLeft.second = true;
        }
    }

    //RIGHT :::::::::::::::::::::::::::::::::::::::::::::

    if (mTileNeighborhood(1, 0) != nullptr
            && mTileNeighborhood(1, 0)->hasTileTemplate()) {
        if (!topLeft.first && !topLeft.second
                && mTileNeighborhood(0, -1) != nullptr
                && mTileNeighborhood(0, -1)->tileTemplate() == mTileNeighborhood(1, 0)->tileTemplate()) {
            //Right and top have the same template
            //Now check if the inner diagonal should be made

            bool botDiff = mTileNeighborhood(0, 1) == nullptr
                    || mTileNeighborhood(0, 1)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool leftDiff = mTileNeighborhood(-1, 0) == nullptr
                    || mTileNeighborhood(-1, 0)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();

            if (botDiff && leftDiff)
                topRight.first = true;

            //Now see if outer diagonal should be made

            bool farTopDiff = mTileNeighborhood(1, -2) == nullptr
                    || mTileNeighborhood(1, -2)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool farRightDiff = mTileNeighborhood(2, -1) == nullptr
                    || mTileNeighborhood(2, -1)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool centerClear = mTileNeighborhood(1, -1) == nullptr
                    || !mTileNeighborhood(1, -1)->hasTileTemplate();

            if (farTopDiff && farRightDiff && centerClear)
                topRight.second = true;
        }

        if (!bottomLeft.first && !bottomLeft.second && !topRight.first && !topRight.second
                && mTileNeighborhood(0, 1) != nullptr
                && mTileNeighborhood(0, 1)->tileTemplate() == mTileNeighborhood(1, 0)->tileTemplate()) {
            //Right and bottom have same template, and no other diagonal conflicts
            //Now check if the inner diagonal should be made

            bool topDiff = mTileNeighborhood(0, -1) == nullptr
                    || mTileNeighborhood(0, -1)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool leftDiff = mTileNeighborhood(-1, 0) == nullptr
                    || mTileNeighborhood(-1, 0)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();

            if (topDiff && leftDiff)
                bottomRight.first = true;

            //Now see if the outer diagonal should be made

            bool farBottomDiff = mTileNeighborhood(1, 2) == nullptr
                    || mTileNeighborhood(1, 2)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool farLeftDiff = mTileNeighborhood(2, 1) == nullptr
                    || mTileNeighborhood(2, 1)->tileTemplate() != mTileNeighborhood(1, 0)->tileTemplate();
            bool centerClear = mTileNeighborhood(1, 1) == nullptr
                    || !mTileNeighborhood(1, 1)->hasTileTemplate();

            if (farBottomDiff && farLeftDiff && centerClear)
                bottomRight.second = true;
        }
    }

    //We now have all the info on which diagonals exist, and need to be constructed

    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> ret;

    const Tile *left = mTileNeighborhood(-1, 0);
    const Tile *right = mTileNeighborhood(1, 0);
    const Tile *top = mTileNeighborhood(0, -1);
    const Tile *bottom = mTileNeighborhood(0, 1);

    if (topLeft.first) {
        QPointF topCenter = top->position().toPointF() + QPointF(0, -1);
        QPointF leftCenter = left->position().toPointF() + QPointF(-1, 0);
        float topHalfThickness = top->thickness() / 2;
        float leftHalfThickness = left->thickness() / 2;

        auto dia = makeDiagonal(
                    leftCenter + QPointF(leftHalfThickness, -leftHalfThickness),
                    leftCenter + QPointF(leftHalfThickness, leftHalfThickness),
                    topCenter + QPointF(topHalfThickness, topHalfThickness),
                    topCenter + QPointF(-topHalfThickness, topHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), left));
    }

    if (topLeft.second) {
        QPointF topCenter = top->position().toPointF() + QPointF(0, -1);
        QPointF leftCenter = left->position().toPointF() + QPointF(-1, 0);
        float topHalfThickness = top->thickness() / 2;
        float leftHalfThickness = left->thickness() / 2;

        auto dia = makeDiagonal(
                    leftCenter + QPointF(-leftHalfThickness, -leftHalfThickness),
                    leftCenter + QPointF(leftHalfThickness, -leftHalfThickness),
                    topCenter + QPointF(-topHalfThickness, topHalfThickness),
                    topCenter + QPointF(-topHalfThickness, -topHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), left));
    }

    if (topRight.first) {
        QPointF topCenter = top->position().toPointF() + QPointF(0, -1);
        QPointF rightCenter = right->position().toPointF() + QPointF(1, 0);
        float topHalfThickness = top->thickness() / 2;
        float rightHalfThickness = right->thickness() / 2;

        auto dia = makeDiagonal(
                    rightCenter + QPointF(-rightHalfThickness, rightHalfThickness),
                    rightCenter + QPointF(-rightHalfThickness, -rightHalfThickness),
                    topCenter + QPointF(topHalfThickness, topHalfThickness),
                    topCenter + QPointF(-topHalfThickness, topHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), right));
    }

    if (topRight.second) {
        QPointF topCenter = top->position().toPointF() + QPointF(0, -1);
        QPointF rightCenter = right->position().toPointF() + QPointF(1, 0);
        float topHalfThickness = top->thickness() / 2;
        float rightHalfThickness = right->thickness() / 2;

        auto dia = makeDiagonal(
                    rightCenter + QPointF(-rightHalfThickness, -rightHalfThickness),
                    rightCenter + QPointF(rightHalfThickness, -rightHalfThickness),
                    topCenter + QPointF(topHalfThickness, -topHalfThickness),
                    topCenter + QPointF(-topHalfThickness, topHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), right));
    }

    if (bottomLeft.first) {
        QPointF bottomCenter = bottom->position().toPointF() + QPointF(0, 1);
        QPointF leftCenter = left->position().toPointF() + QPointF(-1, 0);
        float bottomHalfThickness = bottom->thickness() / 2;
        float leftHalfThickness = left->thickness() / 2;

        auto dia = makeDiagonal(
                    leftCenter + QPointF(leftHalfThickness, -leftHalfThickness),
                    leftCenter + QPointF(leftHalfThickness, leftHalfThickness),
                    bottomCenter + QPointF(-bottomHalfThickness, -bottomHalfThickness),
                    bottomCenter + QPointF(bottomHalfThickness, -bottomHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), left));
    }

    if (bottomLeft.second) {
        QPointF bottomCenter = bottom->position().toPointF() + QPointF(0, 1);
        QPointF leftCenter = left->position().toPointF() + QPointF(-1, 0);
        float bottomHalfThickness = bottom->thickness() / 2;
        float leftHalfThickness = left->thickness() / 2;

        auto dia = makeDiagonal(
                    leftCenter + QPointF(leftHalfThickness, leftHalfThickness),
                    leftCenter + QPointF(-leftHalfThickness, leftHalfThickness),
                    bottomCenter + QPointF(-bottomHalfThickness, bottomHalfThickness),
                    bottomCenter + QPointF(-bottomHalfThickness, -bottomHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), left));
    }

    if (bottomRight.first) {
        QPointF bottomCenter = bottom->position().toPointF() + QPointF(0, 1);
        QPointF rightCenter = right->position().toPointF() + QPointF(1, 0);
        float bottomHalfThickness = bottom->thickness() / 2;
        float rightHalfThickness = right->thickness() / 2;

        auto dia = makeDiagonal(
                    rightCenter + QPointF(-rightHalfThickness, rightHalfThickness),
                    rightCenter + QPointF(-rightHalfThickness, -rightHalfThickness),
                    bottomCenter + QPointF(-bottomHalfThickness, -bottomHalfThickness),
                    bottomCenter + QPointF(bottomHalfThickness, -bottomHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), right));
    }

    if (bottomRight.second) {
        QPointF bottomCenter = bottom->position().toPointF() + QPointF(0, 1);
        QPointF rightCenter = right->position().toPointF() + QPointF(1, 0);
        float bottomHalfThickness = bottom->thickness() / 2;
        float rightHalfThickness = right->thickness() / 2;

        auto dia = makeDiagonal(
                    rightCenter + QPointF(rightHalfThickness, rightHalfThickness),
                    rightCenter + QPointF(-rightHalfThickness, rightHalfThickness),
                    bottomCenter + QPointF(bottomHalfThickness, -bottomHalfThickness),
                    bottomCenter + QPointF(bottomHalfThickness, bottomHalfThickness));

        ret.append(dia);
        heightAndMaterial->append(QVector<const Tile *>(dia.size(), right));
    }
    return ret;
}
