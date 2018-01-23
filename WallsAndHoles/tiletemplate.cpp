#include "tiletemplate.h"

TileTemplate::TileTemplate(float height, float thickness, QObject *parent)
    : QObject(parent)
    , mTileId(-1)
    , mHeight(height)
    , mThickness(thickness) {}

void TileTemplate::setHeight(float height)
{
    mHeight = height;
    emit propertyChanged(mTileId);
}

void TileTemplate::setThickness(float thickness)
{
    mThickness = thickness;
    emit propertyChanged(mTileId);
}
