#include "tilemapselectiontool.h"

TileMapSelectionTool::TileMapSelectionTool(TilePropertyView *propView,
                                           TileMapPreviewGrpahicsItem *previewItem)
    : AbstractTileMapTool(previewItem)
    , mTilePropertyView(propView)
{
}

void TileMapSelectionTool::cellActivated(int x, int y)
{
    clearOverlay();
    Tile& tile = getTileMap()->tileAt(x, y);
    if(tile.hasTileTemplate()){
        mTilePropertyView->setTile(&tile);
        drawOverlay(x, y);
    } else {
        mTilePropertyView->setTile(nullptr);
    }
}

void TileMapSelectionTool::deactivate()
{
    clearOverlay();
    mTilePropertyView->clear();
}

void TileMapSelectionTool::drawOverlay(int x, int y)
{
    mPreviewItem->setRegion(QRect(x, y, 1, 1));
    mPreviewItem->setColor(Qt::gray);
}

void TileMapSelectionTool::clearOverlay()
{
    mPreviewItem->setRegion(QRegion());
}
