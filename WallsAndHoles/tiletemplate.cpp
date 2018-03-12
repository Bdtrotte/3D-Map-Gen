// For std::move
#include <utility>

#include "tiletemplate.h"

TileTemplate::TileTemplate(QColor color,
                           QString name,
                           float height,
                           float thickness,
                           TileMaterial *material,
                           QVector2D position,
                           bool bridgeTiles,
                           bool connectDiagonals,
                           QObject *parent)
    : QObject(parent)
    , mName(name)
    , mHeight(height)
    , mThickness(thickness)
    , mPosition(position)
    , mColor(color)
    , mHasSideMaterial(false)
    , mTopMaterial(nullptr)
    , mSideMaterial(nullptr)
    , mBridgeTiles(bridgeTiles)
    , mConnectDiagonals(connectDiagonals)
{
    setTopMaterial(material);
    setSideMaterial(nullptr);
}


void TileTemplate::setHeight(float height)
{
    if (height == mHeight) return;

    mHeight = height;

    emit exclusivePropertyChanged();
    emit changed();
}

float TileTemplate::setThickness(float thickness)
{
    if (thickness == mThickness) return mThickness;

    if (thickness > 1) thickness = 1;
    else if (thickness < MIN_TILE_THICKNESS) thickness = MIN_TILE_THICKNESS;

    if (mPosition.x() > 0.5) {
        float maxThickness = (1 - mPosition.x()) * 2;
        if (thickness > maxThickness) thickness = maxThickness;
    } else if (mPosition.x() < 0.5) {
        float maxThickness = mPosition.x() * 2;
        if (thickness > maxThickness) thickness = maxThickness;
    }

    if (mPosition.y() > 0.5) {
        float maxThickness = (1 - mPosition.y()) * 2;
        if (thickness > maxThickness) thickness = maxThickness;
    } else if (mPosition.y() < 0.5) {
        float maxThickness = mPosition.y() * 2;
        if (thickness > maxThickness) thickness = maxThickness;
    }

    mThickness = thickness;

    emit thicknessChanged();
    emit changed();

    return mThickness;
}

QVector2D TileTemplate::setPosition(QVector2D position)
{
    if (position == mPosition) return mPosition;

    if (position.x() > 0.5) {
        float maxX = 1 - mThickness / 2;
        if (position.x() > maxX) position.setX(maxX);
    } else if (position.x() < 0.5) {
        float minX = mThickness / 2;
        if (position.x() < minX) position.setX(minX);
    }

    if (position.y() > 0.5) {
        float maxY = 1 - mThickness / 2;
        if (position.y() > maxY) position.setY(maxY);
    } else if (position.y() < 0.5) {
        float minY = mThickness / 2;
        if (position.y() < minY) position.setY(minY);
    }

    mPosition = position;

    emit positionChanged();
    emit changed();

    return mPosition;
}

void TileTemplate::setColor(QColor color)
{
    mColor = color;

    emit exclusivePropertyChanged();
    emit changed();
}

void TileTemplate::setHasSideMaterial(bool enabled)
{
    if (mHasSideMaterial == enabled) return;

    mHasSideMaterial = enabled;

    emit materialChanged();
    emit changed();
}

void TileTemplate::setTopMaterial(TileMaterial *material)
{
    // mMaterial should never be null--there is always some default
    // material information.
    if (material == nullptr)
        material = TileMaterial::getDefaultMaterial();

    if (mTopMaterial)
        disconnect(mTopMaterial);

    mTopMaterial = material;

    // When the material's properties change, a materialChanged() signal will be emitted.
    // The changed() signal is currently only used for saving purposes, so it does not need to be emitted.

    connect(mTopMaterial, &TileMaterial::textureChanged, this, &TileTemplate::materialChanged);
    connect(mTopMaterial, &TileMaterial::phongParamsChanged, this, &TileTemplate::materialChanged);
    connect(mTopMaterial, &TileMaterial::aboutToBeRemoved,
            this, [this]() {
        setTopMaterial(TileMaterial::getDefaultMaterial());
    });

    emit materialChanged();
    emit changed();
}

void TileTemplate::setSideMaterial(TileMaterial *material)
{
    // mMaterial should never be null--there is always some default
    // material information.
    if (material == nullptr)
        material = TileMaterial::getDefaultMaterial();

    if (mSideMaterial)
        disconnect(mSideMaterial);

    mSideMaterial = material;

    // When the material's properties change, a materialChanged() signal will be emitted.
    // The changed() signal is currently only used for saving purposes, so it does not need to be emitted.

    connect(mSideMaterial, &TileMaterial::textureChanged, this, &TileTemplate::materialChanged);
    connect(mSideMaterial, &TileMaterial::phongParamsChanged, this, &TileTemplate::materialChanged);
    connect(mSideMaterial, &TileMaterial::aboutToBeRemoved,
            this, [this]() {
        setSideMaterial(TileMaterial::getDefaultMaterial());
    });

    emit materialChanged();
    emit changed();
}

void TileTemplate::setBridgeTiles(bool enabled)
{
    if (mBridgeTiles == enabled) return;

    mBridgeTiles = enabled;

    emit exclusivePropertyChanged();
    emit changed();
}

void TileTemplate::setConnectDiagonals(bool enabled)
{
    if (mConnectDiagonals == enabled) return;

    mConnectDiagonals = enabled;

    emit exclusivePropertyChanged();
    emit changed();
}
