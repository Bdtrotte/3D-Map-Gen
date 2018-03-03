#include "rectbrushtool.h"

#include "shaperegion.h"

RectBrushTool::RectBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent)
    : AbstractShapeBrushTool(previewItem, parent) {}

QRegion RectBrushTool::getShape(QPoint start, QPoint end) const
{
    return ShapeRegion::rectOutline(start, end);
}
