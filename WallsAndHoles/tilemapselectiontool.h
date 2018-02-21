#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

#include "abstracttilemaptool.h"
#include "propertybrowser.h"
#include "mapview.h"

class TileMapSelectionTool : public AbstractTileMapTool
{
public:
    TileMapSelectionTool(PropertyBrowser *propView, TileMapPreviewGraphicsItem *previewItem, QObject *parent = nullptr);
    void cellActivated(int x, int y) override;
    void deactivate() override;

private:
    void drawOverlay(int endX, int endY);
    void clearOverlay();
    PropertyBrowser* mTilePropertyView;
};

#endif // TILEMAPSELECTIONTOOL_H
