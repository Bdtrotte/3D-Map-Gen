#include <QtMath>


#include "ellipsebrushtool.h"


EllipseBrushTool::EllipseBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractShapeBrushTool(mapView, tileMap, drawWith)
{

}



Array2D<bool> EllipseBrushTool::getShape(int dx, int dy) const {

    if (dx == 0)
        return Array2D<bool>(1, abs(dy) + 1, true);
    if (dy == 0)
        return Array2D<bool>(abs(dx) + 1, 1, true);


    Array2D<bool> shape(abs(dx) + 1, abs(dy) + 1, false);

    float w = shape.width();
    float h = shape.height();

    // Center coordinates.
    float cx = w * 0.5;
    float cy = h * 0.5;

    // Theta will sweep from 0 to 2 pi.
    float theta = 0;


    // x and y "radii"
    float rx = cx;
    float ry = cy;

    float x = cx + rx * cos(theta);
    float y = cy + ry * sin(theta);

    while (theta <= 2 * M_PI) {

        // The x coordinate of the next vertical line to be crossed.
        float nx = theta < M_PI ? floor(x) : ceil(x);

        // The y coordinate of the next horizontal line to be crossed.
        float ny = theta < .5 * M_PI || theta > 1.5 * M_PI ? ceil(y) : floor(y);

        if (x < 1)
            nx = 1;
        if (x > w - 1)
            nx = w - 1;

        if (y < 1)
            ny = 1;
        if (y > h - 1)
            ny = h - 1;

        // Compute the next theta at which an X or Y jump will occur.
        float tx = acos((nx - cx) / rx);
        float ty = asin((ny - cy) / ry);

        // Adjust time values so that they are above theta.
        if (tx < theta)
            tx = 2 * M_PI - tx;
        if (ty < theta)
            ty = M_PI - ty;

        // Pick the minimum next theta.
        float t = std::min(tx, ty);

        // Make sure next theta is a little more than the previous theta.
        theta = std::max(t, theta + 0.001f / std::max(w, h));

        x = cx + rx * cos(theta);
        y = cy + ry * sin(theta);


        int curX = std::min(std::max(floor(x), 0.0), w-1.0);
        int curY = std::min(std::max(floor(y), 0.0), h-1.0);


        shape(curX, curY) = true;
    }

    return shape;
}
