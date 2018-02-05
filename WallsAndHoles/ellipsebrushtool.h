#ifndef ELLIPSEBRUSHTOOL_H
#define ELLIPSEBRUSHTOOL_H

#include "abstractshapebrushtool.h"

class EllipseBrushTool : public AbstractShapeBrushTool {
public:
    EllipseBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawMaterial);

    /// Draws an ellipse.
    Array2D<bool> getShape(int dx, int dy) const override;
};

#endif // ELLIPSEBRUSHTOOL_H
