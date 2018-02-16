#ifndef ABSTRACTTILEMAPTOOL_H
#define ABSTRACTTILEMAPTOOL_H

#include "abstracttool.h"
#include "tilemap.h"
#include "tiletemplate.h"
#include "tilemappreviewgrpahicsitem.h"

class AbstractTileMapTool : public AbstractTool
{
public:
    AbstractTileMapTool(TileMapPreviewGrpahicsItem *previewItem)
        : mPreviewItem(previewItem)
        , mTileMap(nullptr)
        , mTileTemplate(nullptr)
    {
        toolTileMapChanged();
    }

    void setTileMap(TileMap *tileMap)
    {
        TileMap *prev = mTileMap;
        mTileMap = tileMap;
        toolTileMapChanged(prev);
    }

    void setTileTemplate(TileTemplate *tileTemplate) { mTileTemplate = tileTemplate; }


    /**
     * @brief Called whenever mTileMap changes. Called in constructor.
     *
     * @param previous  The OLD mTileMap. The new one can be retreived using getTileMap().
     */
    virtual void toolTileMapChanged(TileMap *previous = nullptr) { Q_UNUSED(previous); }


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


    /**
     * @brief Called when the mouse hovers over a new cell.
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    virtual void cellHovered(int, int) {}


    /**
     * @brief Called when the mouse is no longer hovering over a cell.
     */
    virtual void mouseExitedMap() {}

protected:
    TileMap *getTileMap() const { return mTileMap; }
    TileTemplate *getTileTemplate() const { return mTileTemplate; }

    //the preview item of mapView, can't be changed
    TileMapPreviewGrpahicsItem *const mPreviewItem;

private:
    TileMap *mTileMap;
    TileTemplate *mTileTemplate;

    using AbstractTool::mousePressEvent;
    using AbstractTool::mouseReleaseEvent;
    using AbstractTool::mouseMoveEvent;
    using AbstractTool::wheelEvent;
};

#endif // ABSTRACTTILEMAPTOOL_H
