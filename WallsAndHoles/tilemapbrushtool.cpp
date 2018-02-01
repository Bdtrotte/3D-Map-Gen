#include "tilemapbrushtool.h"

#include <QDebug>

TileMapBrushTool::TileMapBrushTool(TileMap *tileMap)
    : AbstractTileMapTool(tileMap)
{
    mTileTemplate = SharedTileTemplate(new TileTemplate(2, 0, QVector2D(0.5,0.5), Qt::red));
}

void TileMapBrushTool::cellActivated(int x, int y)
{
    mTileMap->setTile(x, y, mTileTemplate);
}
