#include "ellipsebrushtool.h"

#include "shaperegion.h"

EllipseBrushTool::EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent)
    : AbstractShapeBrushTool(previewItem, parent) {}


QRegion EllipseBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::ellipseOutline(start, end);
}
