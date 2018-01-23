#include "tile.h"

Tile::Tile(const SharedTileTemplate tileTemplate,
               int xPos,
               int yPos,
               float relativeSize,
               float relativeHeight,
               QVector2D relativePosition,
               QObject *parent)
    : QObject(parent)
    , mTileTemplate(tileTemplate)
    , mXPos(xPos)
    , mYPos(yPos)
    , mRelativeSize(relativeSize)
    , mRelativeHeight(relativeHeight)
    , mRelativePosition(relativePosition) {}
