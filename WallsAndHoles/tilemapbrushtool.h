#ifndef TILEMAPBRUSHTOOL_H
#define TILEMAPBRUSHTOOL_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

class TileMapBrushTool : public AbstractTileMapTool
{
public:
    TileMapBrushTool(TileMapPreviewGrpahicsItem *previewItem);

    void cellActivated(int x, int y) override;
    void cellHovered(int x, int y) override;
    void mouseExitedMap();
};

#endif // TILEMAPBRUSHTOOL_H
