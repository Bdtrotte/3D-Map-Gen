#include "tilemapselectiontool.h"
#include "tilepropertymanager.h"

TileMapSelectionTool::TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                                           TileMapPreviewGraphicsItem *previewItem,
                                           QObject *parent)
    : AbstractTileSelectionTool(propertyBrowser, previewItem, parent) {}

void TileMapSelectionTool::cellActivated(int x, int y)
{
    mSelection += QRect(x, y, 1, 1);

    activateSelection();
}
