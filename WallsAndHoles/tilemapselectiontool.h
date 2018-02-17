#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

#include "abstracttilemaptool.h"
#include "tilepropertyview.h"
#include "mapview.h"

class TileMapSelectionTool : public AbstractTileMapTool
{
public:
    TileMapSelectionTool(TilePropertyView *propView, TileMapPreviewGraphicsItem *previewItem);
    void cellActivated(int x, int y) override;
    void deactivate() override;

private:
    void drawOverlay(int endX, int endY);
    void clearOverlay();
    TilePropertyView* mTilePropertyView;
};

#endif // TILEMAPSELECTIONTOOL_H
