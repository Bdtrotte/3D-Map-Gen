#include "rectbrushtool.h"

#include "shaperegion.h"

RectBrushTool::RectBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack)
    : AbstractShapeBrushTool(previewItem, undoStack) {}

QRegion RectBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::rectOutline(start, end);
}
