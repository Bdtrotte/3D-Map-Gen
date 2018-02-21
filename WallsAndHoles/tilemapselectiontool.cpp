#include "tilemapselectiontool.h"
#include "tilepropertymanager.h"

TileMapSelectionTool::TileMapSelectionTool(PropertyBrowser *propView,
                                           TileMapPreviewGraphicsItem *previewItem,
                                           QObject *parent)
    : AbstractTileMapTool(previewItem, parent)
    , mTilePropertyView(propView) {}

void TileMapSelectionTool::cellActivated(int x, int y)
{
    clearOverlay();
    Tile &tile = getTileMap()->tileAt(x, y);
    if(tile.hasTileTemplate()){
        mTilePropertyView->setPropertyManager(new TilePropertyManager(tile));
        drawOverlay(x, y);
    } else {
        mTilePropertyView->clear();
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
