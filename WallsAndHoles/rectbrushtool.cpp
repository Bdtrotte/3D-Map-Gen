#include "rectbrushtool.h"

#include "shaperegion.h"

RectBrushTool::RectBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractShapeBrushTool(previewItem, undoStack, parent) {}

QRegion RectBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::rectOutline(start, end);
}
