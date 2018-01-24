#include "tiletemplate.h"

TileTemplate::TileTemplate(float height,
                           float thickness,
                           QVector2D position,
                           QObject *parent)
    : QObject(parent)
    , mTileId(-1)
    , mHeight(height)
    , mThickness(thickness)
    , mPosition(position) {}

void TileTemplate::setHeight(float height)
{
    mHeight = height;

    emit propertyChanged(mTileId);
}

void TileTemplate::setThickness(float thickness)
{
    Q_ASSERT(thickness > 0 && thickness <= 1);

    mThickness = thickness;

    emit propertyChanged(mTileId);
}

void TileTemplate::setPosition(QVector2D position)
{
    Q_ASSERT(position.x() > 0
             && position.x() < 1
             && position.y() > 0
             && position.y() < 1);

    mPosition = position;

    emit propertyChanged(mTileId);
}
