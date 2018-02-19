#include "tilepropertymanager.h"

TilePropertyManager::TilePropertyManager(Tile &tile)
    : AbstractPropertyManager()
    , mTile(tile)
{
    connect(&mTile, &Tile::tileChanged,
            this, &TilePropertyManager::tileChanged);
}

void TilePropertyManager::propertyEdited(QString propertyName, QVariant value)
{
    if        (propertyName == "Relative Height")     {
        mTile.setRelativeHeight(value.toFloat());
    } else if (propertyName == "Relative Thickness")  {
        mTile.setRelativeThickness(value.toFloat());
    } else if (propertyName == "Relative X Position") {
        QVector2D pos = mTile.relativePosition();
        pos.setX(value.toFloat());
        mTile.setRelativePosition(pos);
    } else if (propertyName == "Relative Y Position") {
        QVector2D pos = mTile.relativePosition();
        pos.setY(value.toFloat());
        mTile.setRelativePosition(pos);
    }
}

QVector<QVector<QVariant> > TilePropertyManager::properties()
{
    return {
        {"Relative Height",     mTile.relativeHeight(),       true, -999, 999},
        {"Relative Thickness",  mTile.relativeThickness(),    false,   0,   1},
        {"Relative X Position", mTile.relativePosition().x(), false,  -1,   1},
        {"Relative Y Position", mTile.relativePosition().y(), false,  -1,   1},
        {"Tile Template",       mTile.hasTileTemplate()? mTile.tileTemplate()->name() : "No Template", false}
    };
}

void TilePropertyManager::tileChanged()
{
    emit propertyChanged("Relative Height", mTile.relativeHeight());
    emit propertyChanged("Relative Thickness", mTile.relativeThickness());
    emit propertyChanged("Relative X Position", mTile.relativePosition().x());
    emit propertyChanged("Relative Y Position", mTile.relativePosition().y());
    emit propertyChanged("Tile Template", mTile.hasTileTemplate()? mTile.tileTemplate()->name() : "No Template");
}
