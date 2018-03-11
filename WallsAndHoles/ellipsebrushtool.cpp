#include "ellipsebrushtool.h"

#include "shaperegion.h"

EllipseBrushTool::EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack)
    : AbstractShapeBrushTool(previewItem, undoStack) {}


QRegion EllipseBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::ellipseOutline(start, end);
}
