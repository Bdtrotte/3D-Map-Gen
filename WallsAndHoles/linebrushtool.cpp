#include "linebrushtool.h"

#include "shaperegion.h"

LineBrushTool::LineBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent)
    : AbstractShapeBrushTool(previewItem, parent) {}

QRegion LineBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::line(start, end);
}
