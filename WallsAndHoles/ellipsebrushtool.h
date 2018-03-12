#ifndef ELLIPSEBRUSHTOOL_H
#define ELLIPSEBRUSHTOOL_H

#include "abstractshapebrushtool.h"

class EllipseBrushTool : public AbstractShapeBrushTool {
public:
    EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack);

    /// Draws an ellipse.
    QRegion getShape(QPoint start, QPoint end) const override;
};

#endif // ELLIPSEBRUSHTOOL_H
