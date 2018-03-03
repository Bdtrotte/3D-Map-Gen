#include "tilepropertymanager.h"

TilePropertyManager::TilePropertyManager(QVector<Tile *> tiles)
    : AbstractPropertyManager()
    , mTiles(tiles)
{
    // TODO connect Tile changes to this
}

void TilePropertyManager::propertyEdited(QString propertyName, QVariant value)
{
    if (propertyName == "Relative Height") {
        for (Tile *t : mTiles)
            t->setRelativeHeight(value.toFloat());
    } else if (propertyName == "Relative Thickness")  {
        for (Tile *t : mTiles)
            t->setRelativeThickness(value.toFloat());
    } else if (propertyName == "Relative X Position") {
        for (Tile *t : mTiles) {
            QVector2D pos = t->relativePosition();
            pos.setX(value.toFloat());
            t->setRelativePosition(pos);
        }
    } else if (propertyName == "Relative Y Position") {
        for (Tile *t : mTiles) {
            QVector2D pos = t->relativePosition();
            pos.setY(value.toFloat());
            t->setRelativePosition(pos);
        }
    }
}

QVector<QVector<QVariant>> TilePropertyManager::properties()
{
    Tile *baseTile = mTiles.first();
    bool heightDiff, thicknessDiff, xPosDiff, yPosDiff;
    heightDiff = thicknessDiff = xPosDiff = yPosDiff = false;

    for (auto i = mTiles.begin() + 1; i != mTiles.end(); ++i) {
        if (baseTile->relativeHeight() != (*i)->relativeHeight())
            heightDiff = true;
        if (baseTile->relativeThickness() != (*i)->relativeThickness())
            thicknessDiff = true;
        if (baseTile->relativePosition().x() != (*i)->relativePosition().x())
            xPosDiff = true;
        if (baseTile->relativePosition().y() != (*i)->relativePosition().y())
            yPosDiff = true;
    }

    return {
        {"Relative Height",     baseTile->relativeHeight(),       true, -999, 999, heightDiff},
        {"Relative Thickness",  baseTile->relativeThickness(),    false,   0,   1, thicknessDiff},
        {"Relative X Position", baseTile->relativePosition().x(), false,  -1,   1, xPosDiff},
        {"Relative Y Position", baseTile->relativePosition().y(), false,  -1,   1, yPosDiff},
        {"Tile Template",       mTiles.size() > 1? "--" :
                                                  (baseTile->hasTileTemplate()? baseTile->tileTemplate()->name() : "No Template"), false}
    };
}
