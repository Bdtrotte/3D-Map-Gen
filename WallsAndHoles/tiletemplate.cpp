// For std::move
#include <utility>

#include "tiletemplate.h"

TileTemplate::TileTemplate(QColor color,
                           QString name,
                           float height,
                           float thickness,
                           TileMaterial *material,
                           QVector2D position,
                           QObject *parent)
    : QObject(parent)
    , mName(name)
    , mHeight(height)
    , mThickness(thickness)
    , mPosition(position)
    , mColor(color)
{
    setMaterial(material);
}


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

void TileTemplate::setMaterial(TileMaterial *material)
{
    // mMaterial should never be null--there is always some default
    // material information.
    if (material == nullptr)
        material = TileMaterial::getDefaultMaterial();

    mMaterial = material;


    // When the material's properties change, a materialChanged() signal will be emitted.
    // The changed() signal is currently only used for saving purposes, so it does not need to be emitted.

    connect(mMaterial, &TileMaterial::textureChanged, this, &TileTemplate::materialChanged);
    connect(mMaterial, &TileMaterial::phongParamsChanged, this, &TileTemplate::materialChanged);

    emit materialChanged();
    emit changed();
}
