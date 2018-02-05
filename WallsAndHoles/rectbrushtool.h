#ifndef RECTBRUSHTOOL_H
#define RECTBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class RectBrushTool : public AbstractShapeBrushTool {
public:
    RectBrushTool(TileMap *tileMap, SharedTileTemplate drawWith);

    /// @brief Draws a rectangle.
    Array2D<bool> getShape(int dx, int dy) const override;
};

#endif // RECTBRUSHTOOL_H
