#include "rectbrushtool.h"

RectBrushTool::RectBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractShapeBrushTool(mapView, tileMap, drawWith)
{

}



Array2D<bool> RectBrushTool::getShape(int dx, int dy) const {
    Array2D<bool> shape(abs(dx) + 1, abs(dy) + 1, false);


    for (int x = 0; x < shape.width(); ++x) {
        shape(x, 0) = true;
        shape(x, abs(dy)) = true;
    }

    for (int y = 0; y < shape.height(); ++y) {
        shape(0, y) = true;
        shape(abs(dx), y) = true;
    }


    return shape;
}
