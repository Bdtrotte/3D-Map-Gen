#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H
#include "abstracttilemaptool.h"
#include "tilepropertyview.h"
#include "mapview.h"
#include "mapoverlaycell.h"

class TileMapSelectionTool : public AbstractTileMapTool
{
public:
    TileMapSelectionTool(TilePropertyView *propView, MapView *mapView, TileMap *tileMap);
    void cellActivated(int x, int y) override;
    void deactivate() override;

private:
    void drawOverlay(int endX, int endY);
    void clearOverlay();
    TilePropertyView* mTilePropertyView;
    MapView* mMapView;
    QSharedPointer<MapOverlayCell> mOverlay;
};

#endif // TILEMAPSELECTIONTOOL_H
