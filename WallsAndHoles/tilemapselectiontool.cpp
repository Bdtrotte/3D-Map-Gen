#include "tilemapselectiontool.h"

TileMapSelectionTool::TileMapSelectionTool(TilePropertyView *propView, MapView* mapView, TileMap *tileMap)
    : AbstractTileMapTool(tileMap)
    , mTilePropertyView(propView)
    , mMapView(mapView)
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

void TileMapSelectionTool::drawOverlay(int x, int y) {
    clearOverlay();

    QGraphicsScene *scene = mMapView->scene();
    mOverlay = QSharedPointer<MapOverlayCell>::create(scene, x, y, QColor(0, 0, 0, 50));
}

void TileMapSelectionTool::clearOverlay() {
    // Clears overlay.
    mOverlay = QSharedPointer<MapOverlayCell>();
}
