// For std::max()
#include <algorithm>

#include <QtMath>

#include "linebrushtool.h"

LineBrushTool::LineBrushTool(TileMap *tileMap, SharedTileTemplate drawMaterial)
    : AbstractShapeBrushTool(tileMap, drawMaterial)
{

}



Array2D<bool> LineBrushTool::getShape(int dx, int dy) const {


    // Vertical line.
    if (dx == 0)
        return Array2D<bool>(1, abs(dy) + 1, true);

    // Horizontal line.
    if (dy == 0)
        return Array2D<bool>(abs(dx) + 1, 1, true);


    // Diagonal line.
    Array2D<bool> shape(abs(dx) + 1, abs(dy) + 1, false);

    int startX = std::max(0, -dx);
    int startY = std::max(0, -dy);


    float x = startX;
    float y = startY;

    while (true) {
        float ox = x + 0.5f;
        float oy = y + 0.5f;

        // "Time" to next X or Y jump.
        float tx = std::max( (ceil(ox) - ox) / float(dx), (floor(ox) - ox) / float(dx) );
        float ty = std::max( (ceil(oy) - oy) / float(dy), (floor(oy) - oy) / float(dy) );

        float t = std::min(tx, ty);

        // Make sure t is not less than 0.001 of a tile, lest an infinite loop happens.
        t = std::max(t, 0.001f / std::max(abs(dx), abs(dy)));

        x = x + t * dx;
        y = y + t * dy;

        int curX = round(x);
        int curY = round(y);

        // Check (curX, curY) is in bounds.
        if (curX < 0 || curX > dx)
            break;
        if (curY < 0 || curY > dy)
            break;

        shape(curX, curY) = true;
    }

    return shape;
}
