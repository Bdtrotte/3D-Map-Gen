#include "ellipsebrushtool.h"

#include "shaperegion.h"

EllipseBrushTool::EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractShapeBrushTool(previewItem, undoStack, parent) {}


QRegion EllipseBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::ellipseOutline(start, end);
}
