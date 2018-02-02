#include "tilemapbrushtool.h"

#include <QDebug>

TileMapBrushTool::TileMapBrushTool(TileMap *tileMap, SharedTileTemplate tileTemplate)
    : AbstractTileMapTool(tileMap)
    , mTileTemplate(tileTemplate) {}

void TileMapBrushTool::cellActivated(int x, int y)
{
    mTileMap->setTile(x, y, mTileTemplate);
}
