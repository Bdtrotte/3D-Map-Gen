#include "tiletemplate.h"

TileTemplate::TileTemplate(float height,
                           float thickness,
                           QVector2D position, QColor color,
                           QObject *parent)
    : QObject(parent)
    , mHeight(height)
    , mThickness(thickness)
    , mPosition(position)
    , mColor(color) {}

void TileTemplate::setHeight(float height)
{
    mHeight = height;

    emit exclusivePropertyChanged();
}

void TileTemplate::setThickness(float thickness)
{
    Q_ASSERT(thickness > 0 && thickness <= 1);

    mThickness = thickness;

    emit thicknessChanged();
}

void TileTemplate::setPosition(QVector2D position)
{
    Q_ASSERT(position.x() > 0
             && position.x() < 1
             && position.y() > 0
             && position.y() < 1);

    mPosition = position;

    emit positionChanged();
}

void TileTemplate::setColor(QColor color)
{
    mColor = color;

    emit exclusivePropertyChanged();
}
