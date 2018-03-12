#include "linebrushtool.h"

#include "shaperegion.h"

LineBrushTool::LineBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack)
    : AbstractShapeBrushTool(previewItem, undoStack) {}

QRegion LineBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::line(start, end);
}
