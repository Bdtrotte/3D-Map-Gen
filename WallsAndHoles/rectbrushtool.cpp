#include "rectbrushtool.h"

#include "shaperegion.h"

RectBrushTool::RectBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractShapeBrushTool(previewItem) {}

QRegion RectBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::rectOutline(start, end);
}
