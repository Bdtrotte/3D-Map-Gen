#include "tilemapselectiontool.h"

TileMapSelectionTool::TileMapSelectionTool(TilePropertyView *view,TileMap *tileMap)
    : AbstractTileMapTool(tileMap)
    , mTilePropertyView(view)
{

}
