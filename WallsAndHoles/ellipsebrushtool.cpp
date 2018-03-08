#include "ellipsebrushtool.h"

#include "shaperegion.h"

EllipseBrushTool::EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractShapeBrushTool(previewItem) {}


QRegion EllipseBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::ellipseOutline(start, end);
}
