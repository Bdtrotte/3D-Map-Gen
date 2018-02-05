#ifndef ABSTRACTSHAPEBRUSHTOOL_H
#define ABSTRACTSHAPEBRUSHTOOL_H

#include "array2d.h"
#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"

class AbstractShapeBrushTool : public AbstractTileMapTool {
public:

    AbstractShapeBrushTool(TileMap *tileMap, SharedTileTemplate drawWith);


    void cellClicked(int x, int y) override;
    void cellActivated(int x, int y) override;
    void cellReleased(int x, int y) override;


    /**
     * @brief This function should create an Array2D of size (abs(dx)+1, abs(dy)+1)
     * that is TRUE wherever a tile will be updated and FALSE elsewhere.
     *
     * The array should be filled with the assumption that...
     *
     * ...if dx > 0 and dy > 0, then drawing started at (0, 0).
     *
     * ...if dx > 0 and dy < 0, then drawing started at (0, -dy).
     *
     * ...if dx < 0 and dy > 0, then drawing started at (-dx, 0).
     *
     * ...if dx < 0 and dy < 0, then drawing started at (-dx, -dy).
     *
     * @param dx  The X offset from the start of drawing.
     * @param dy  The Y offset from the start of drawing.
     */
    virtual Array2D<bool> getShape(int dx, int dy) const = 0;

private:
    int mStartX;  /// The X position of the first click.
    int mStartY;  /// The Y position of the first click.

    /// The tile template that will be placed down on the map.
    SharedTileTemplate mDrawMaterial;

    /// Draws an overlay previewing the shape that will be drawn.
    void drawOverlay(int endX, int endY);

    /// Clears the overlay.
    void clearOverlay();

    /// Places down a shape.
    void placeShape(int endX, int endY);
};

#endif // ABSTRACTSHAPEBRUSHTOOL_H
