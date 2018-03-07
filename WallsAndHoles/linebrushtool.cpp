#include "linebrushtool.h"

#include "shaperegion.h"

LineBrushTool::LineBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractShapeBrushTool(previewItem, undoStack, parent) {}

QRegion LineBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::line(start, end);
}
