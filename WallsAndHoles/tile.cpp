#include "tile.h"

#include <QDebug>

Tile::Tile(SharedTileTemplate tileTemplate,
               int xPos,
               int yPos,
               QObject *parent)
    : QObject(parent)
    , mTileTemplate(tileTemplate)
    , mXPos(xPos)
    , mYPos(yPos)
    , mRelativeThickness(0)
    , mRelativeHeight(0)
    , mRelativePosition(QVector2D())
{
    if (!mTileTemplate.isNull()) {
        connect(mTileTemplate.data(), &TileTemplate::exclusivePropertyChanged,
                this, [this]{ emit tileChanged(mXPos, mYPos); });
        connect(mTileTemplate.data(), &TileTemplate::thicknessChanged,
                this, &Tile::templateThicknessChanged);
        connect(mTileTemplate.data(), &TileTemplate::positionChanged,
                this, &Tile::templatePositionChanged);
    }
}

void Tile::setRelativeThickness(float relativeThickness)
{
    Q_ASSERT(relativeThickness > 0 && relativeThickness <= 1);

    if (!mTileTemplate.isNull()) {
        if (relativeThickness + mTileTemplate->thickness() > 1)
            relativeThickness = 1 - mTileTemplate->thickness();
        else if (relativeThickness + mTileTemplate->thickness() < MIN_TILE_THICKNESS)
            relativeThickness = -mTileTemplate->thickness() + MIN_TILE_THICKNESS;

        QVector2D pos = mRelativePosition + mTileTemplate->position();
        float thickness = relativeThickness + mTileTemplate->thickness();

        if (thickness/2 + pos.x() > 1)
            relativeThickness = 2 - 2 * pos.x() - mTileTemplate->thickness();
        else if (-thickness/2 + pos.x() < 0)
            relativeThickness = 2 * pos.x() - mTileTemplate->thickness();

        thickness = relativeThickness + mTileTemplate->thickness();

        if (thickness/2 + pos.y() > 1)
            relativeThickness = 2 - 2 * pos.y() - mTileTemplate->thickness();
        else if (-thickness/2 + pos.y() < 0)
            relativeThickness = 2 * pos.y() - mTileTemplate->thickness();
    }

    mRelativeThickness = relativeThickness;

    emit tileChanged(mXPos, mYPos);
}

void Tile::setRelativeHeight(float relativeHeight)
{
    mRelativeHeight = relativeHeight;

    emit tileChanged(mXPos, mYPos);
}

void Tile::setRelativePosition(QVector2D relavtivePosition)
{
    Q_ASSERT(relavtivePosition.x() < 0.5
             && relavtivePosition.x() > -0.5
             && relavtivePosition.y() < 0.5
             && relavtivePosition.y() > -0.5);

    if (!mTileTemplate.isNull()) {
        float thickness = mRelativeThickness + mTileTemplate->thickness();
        QVector2D pos = relavtivePosition + mTileTemplate->position();

        if (thickness/2 + pos.x() > 1)
            relavtivePosition.setX(1 - thickness/2 - mTileTemplate->position().x());
        else if (-thickness/2 + pos.x() < 0)
            relavtivePosition.setX(thickness/2 - mTileTemplate->position().x());

        if (thickness/2 + pos.y() > 1)
            relavtivePosition.setY(1 - thickness/2 - mTileTemplate->position().y());
        else if (-thickness/2 + pos.y() < 0)
            relavtivePosition.setY(thickness/2 - mTileTemplate->position().y());
    }

    mRelativePosition = relavtivePosition;

    emit tileChanged(mXPos, mYPos);
}

void Tile::resetTile(SharedTileTemplate newTileTemplate)
{
    mRelativeThickness = mRelativeHeight = 0;
    mRelativePosition = QVector2D();

    if (!mTileTemplate.isNull())
        mTileTemplate->disconnect(this);

    mTileTemplate = newTileTemplate;
    if (!mTileTemplate.isNull()) {
        connect(mTileTemplate.data(), &TileTemplate::exclusivePropertyChanged,
                this, [this]{ emit tileChanged(mXPos, mYPos); });
        connect(mTileTemplate.data(), &TileTemplate::thicknessChanged,
                this, &Tile::templateThicknessChanged);
        connect(mTileTemplate.data(), &TileTemplate::positionChanged,
                this, &Tile::templatePositionChanged);
    }

    emit tileChanged(mXPos, mYPos);
}
