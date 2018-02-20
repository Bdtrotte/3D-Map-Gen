// For std::move
#include <utility>

#include "tiletemplate.h"

TileTemplate::TileTemplate(QColor color,
                           QString name,
                           float height,
                           float thickness,
                           QVector2D position,
                           QObject *parent)
    : TileTemplate( // Forward arguments to the more general constructor.
          std::make_unique<SimpleTexturedMaterial>(getDefaultTexture(), 1, 1, 1, 1),
          color,
          name,
          height,
          thickness,
          position,
          parent) {}

TileTemplate::TileTemplate(std::unique_ptr<SimpleTexturedMaterial> &&material,
                           QColor color,
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
    , mColor(color)
    , mMaterial(std::move(material)) {}

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

void TileTemplate::setMaterial(std::unique_ptr<SimpleTexturedMaterial> &&material)
{
    mMaterial = std::move(material);

    emit materialChanged();
    emit changed();
}



QSharedPointer<QImage> TileTemplate::DefaultTexture = nullptr;
QSharedPointer<QImage> TileTemplate::getDefaultTexture()
{
    if (DefaultTexture == nullptr)
        DefaultTexture = QSharedPointer<QImage>::create(":/textures/exampleTexture2.png");

    return DefaultTexture;
}
