#include "tilemapbrushtool.h"

#include <QDebug>

TileMapBrushTool::TileMapBrushTool(TileMap *tileMap)
    : AbstractTileMapTool(tileMap) {}

void TileMapBrushTool::cellActivated(int x, int y)
{
    mTileMap->setTile(x, y, mTileTemplate);
}
