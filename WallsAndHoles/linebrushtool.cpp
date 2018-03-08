#include "linebrushtool.h"

#include "shaperegion.h"

LineBrushTool::LineBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractShapeBrushTool(previewItem) {}

QRegion LineBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::line(start, end);
}
