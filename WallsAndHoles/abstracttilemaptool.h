#ifndef ABSTRACTTILEMAPTOOL_H
#define ABSTRACTTILEMAPTOOL_H

#include "abstracttool.h"
#include "tilemap.h"

class AbstractTileMapTool : public AbstractTool
{
public:
    AbstractTileMapTool(TileMap *tileMap) : mTileMap(tileMap) {}

    void setTileMap(TileMap *tileMap) { mTileMap = tileMap; }

    virtual void cellActivated(int, int) {}
    virtual void cellClicked(int, int)   {}
    virtual void cellReleased(int, int)  {}

protected:
    TileMap *mTileMap;

private:
    using AbstractTool::mousePressEvent;
    using AbstractTool::mouseReleaseEvent;
    using AbstractTool::mouseMoveEvent;
    using AbstractTool::wheelEvent;
};

#endif // ABSTRACTTILEMAPTOOL_H
