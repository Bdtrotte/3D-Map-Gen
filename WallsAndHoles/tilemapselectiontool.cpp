#include "tilemapselectiontool.h"

TileMapSelectionTool::TileMapSelectionTool(TilePropertyView *view,TileMap *tileMap)
    : AbstractTileMapTool(tileMap)
    , mTilePropertyView(view)
{
}

void TileMapSelectionTool::cellActivated(int x, int y)
{
    Tile& tile = mTileMap->tileAt(x, y);
    mTilePropertyView->setTile(tile);
}

void TileMapSelectionTool::deactivate()
{
    mTilePropertyView->clear();
}
