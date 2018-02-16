// For std::max()
#include <algorithm>

#include <QtMath>

#include "linebrushtool.h"

LineBrushTool::LineBrushTool(TileMapPreviewGrpahicsItem *previewItem)
    : AbstractShapeBrushTool(previewItem)
{

}

QVector<QPoint> LineBrushTool::getShape(int dx, int dy) const {

    QVector<QPoint> points;


    // Vertical line.
    if (dx == 0) {
        for (int y = 0; y <= abs(dy); ++y)
            points.push_back(QPoint(0, dy < 0 ? -y : y));
        return points;
    }

    // Horizontal line.
    if (dy == 0) {
        for (int x = 0; x <= abs(dx); ++x)
            points.push_back(QPoint(dx < 0 ? -x : x, 0));
        return points;
    }


    // Diagonal line.

    float x = 0;
    float y = 0;

    points.push_back(QPoint(0, 0));

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


        // Check that we are still in bounds. Finish when the next point is past (dx, dy).
        if (dx < 0) {
            if (curX < dx)
                break;
        } else {
            if (curX > dx)
                break;
        }

        if (dy < 0) {
            if (curY < dy)
                break;
        } else {
            if (curY > dy)
                break;
        }

        points.push_back(QPoint(curX, curY));
    }

    return points;
}
