#include "rectbrushtool.h"

RectBrushTool::RectBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractShapeBrushTool(previewItem) {}

QVector<QPoint> RectBrushTool::getShape(int dx, int dy) const {
    QVector<QPoint> points;


    for (int x = 0; x < abs(dx) + 1; ++x) {
        int px = dx < 0 ? -x : x;

        points.push_back(QPoint(px, 0));
        points.push_back(QPoint(px, dy));
    }

    for (int y = 1; y < abs(dy); ++y) {
        int py = dy < 0 ? -y : y;

        points.push_back(QPoint(0, py));
        points.push_back(QPoint(dx, py));
    }


    return points;
}
