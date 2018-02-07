#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H
#include "abstracttilemaptool.h"
#include "tilepropertyview.h"

class TileMapSelectionTool : public AbstractTileMapTool
{
public:
    TileMapSelectionTool(TilePropertyView *view,TileMap *tileMap);

private:
    TilePropertyView* mTilePropertyView;
};

#endif // TILEMAPSELECTIONTOOL_H
