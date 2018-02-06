#ifndef ABSTRACTTILEMAPTOOL_H
#define ABSTRACTTILEMAPTOOL_H

#include "abstracttool.h"
#include "tilemap.h"

class AbstractTileMapTool : public AbstractTool
{
public:
    AbstractTileMapTool(TileMap *tileMap) : mTileMap(tileMap) {}

    void setTileMap(TileMap *tileMap) { mTileMap = tileMap; }

    /**
     * @brief Called when the left mouse button is down over a new cell.
     *
     * This function is invoked when the mouse button is pressed down or when the mouse
     * moves over a new cell while pressed down.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    virtual void cellActivated(int, int) {}

    /**
     * @brief Called the first time the left mouse button is pressed over a cell.
     *
     * Unlike cellActivated(), this function is called when the mouse button is first pressed
     * and not called for other cells if the mouse is dragged afterward.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    virtual void cellClicked(int, int) {}

    /**
     * @brief Called when the left mouse button is released over a cell.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    virtual void cellReleased(int, int) {}

protected:
    TileMap *mTileMap;

private:
    using AbstractTool::mousePressEvent;
    using AbstractTool::mouseReleaseEvent;
    using AbstractTool::mouseMoveEvent;
    using AbstractTool::wheelEvent;
};

#endif // ABSTRACTTILEMAPTOOL_H
