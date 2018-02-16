#include "tilemapbrushtool.h"

TileMapBrushTool::TileMapBrushTool(TileMapPreviewGrpahicsItem *previewItem)
    : AbstractTileMapTool(previewItem) {}

void TileMapBrushTool::cellActivated(int x, int y)
{
    getTileMap()->setTile(x, y, getTileTemplate());
}

void TileMapBrushTool::cellHovered(int x, int y)
{
    if (TileTemplate *t = getTileTemplate()) {
        mPreviewItem->setRegion(QRect(x, y, 1, 1));
        mPreviewItem->setColor(t->color());
    } else {
        mPreviewItem->setRegion(QRegion());
    }
}

void TileMapBrushTool::mouseExitedMap()
{
    mPreviewItem->setRegion(QRegion());
}
