#include "tilemapbrushtool.h"

#include <QDebug>

TileMapBrushTool::TileMapBrushTool(TileMap *tileMap)
    : AbstractTileMapTool(tileMap) {}

void TileMapBrushTool::cellActivated(int x, int y)
{
    getTileMap()->setTile(x, y, getTileTemplate());
}
