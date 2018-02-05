#ifndef LINEBRUSHTOOL_H
#define LINEBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class LineBrushTool : public AbstractShapeBrushTool {
public:
    LineBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawMaterial);

    /**
     * @brief Makes a 1-pixel line.
     */
    Array2D<bool> getShape(int dx, int dy) const override;
};

#endif // LINEBRUSHTOOL_H
