#ifndef TILEMAPBRUSHTOOL_H
#define TILEMAPBRUSHTOOL_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

class TileMapBrushTool : public AbstractTileMapTool
{
public:
    TileMapBrushTool(TileMap *tileMap, SharedTileTemplate tileTemplate);

    void cellActivated(int x, int y) override;

private:
    SharedTileTemplate mTileTemplate;
};

#endif // TILEMAPBRUSHTOOL_H
