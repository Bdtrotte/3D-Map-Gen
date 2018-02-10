#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H
#include "abstracttilemaptool.h"
#include "tilepropertyview.h"

class TileMapSelectionTool : public AbstractTileMapTool
{
public:
    TileMapSelectionTool(TilePropertyView *view,TileMap *tileMap);
    void cellActivated(int x, int y) override;
    void deactivate() override;

private:
    TilePropertyView* mTilePropertyView;
};

#endif // TILEMAPSELECTIONTOOL_H
