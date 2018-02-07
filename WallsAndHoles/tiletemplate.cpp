#include "tiletemplate.h"

TileTemplate::TileTemplate(QColor color,
                           QString name,
                           float height,
                           float thickness,
                           QVector2D position,
                           QObject *parent)
    : QObject(parent)
    , mName(name)
    , mHeight(height)
    , mThickness(thickness)
    , mPosition(position)
    , mColor(color) {}

void TileTemplate::setHeight(float height)
{
    if (height == mHeight) return;

    mHeight = height;

    emit exclusivePropertyChanged();
    emit changed();
}

void TileTemplate::setThickness(float thickness)
{
    if (thickness == mThickness) return;

    Q_ASSERT(thickness > 0 && thickness <= 1);

    mThickness = thickness;

    emit thicknessChanged();
    emit changed();
}

void TileTemplate::setPosition(QVector2D position)
{
    if (position == mPosition) return;

    Q_ASSERT(position.x() > 0
             && position.x() < 1
             && position.y() > 0
             && position.y() < 1);

    mPosition = position;

    emit positionChanged();
    emit changed();
}

void TileTemplate::setColor(QColor color)
{
    mColor = color;

    emit exclusivePropertyChanged();
    emit changed();
}
