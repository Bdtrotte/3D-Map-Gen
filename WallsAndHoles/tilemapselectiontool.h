#ifndef TILEMAPSELECTIONTOOL_H
#define TILEMAPSELECTIONTOOL_H

#include "abstracttileselectiontool.h"

class TileMapSelectionTool : public AbstractTileSelectionTool
{
public:
    TileMapSelectionTool(PropertyBrowser *propertyBrowser,
                         TileMapPreviewGraphicsItem *previewItem,
                         QObject *parent = nullptr);

    void cellActivated(int x, int y) override;
};

#endif // TILEMAPSELECTIONTOOL_H
