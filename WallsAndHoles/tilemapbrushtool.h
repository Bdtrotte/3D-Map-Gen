#ifndef TILEMAPBRUSHTOOL_H
#define TILEMAPBRUSHTOOL_H

#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

class TileMapBrushTool : public AbstractTileMapTool
{
public:
    TileMapBrushTool(TileMap *tileMap);

    void cellActivated(int x, int y) override;
};

#endif // TILEMAPBRUSHTOOL_H
