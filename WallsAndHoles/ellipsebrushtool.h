#ifndef ELLIPSEBRUSHTOOL_H
#define ELLIPSEBRUSHTOOL_H

#include "abstractshapebrushtool.h"

class EllipseBrushTool : public AbstractShapeBrushTool {
public:
    EllipseBrushTool(TileMapPreviewGraphicsItem *previewItem);

    /// Draws an ellipse.
    QVector<QPoint> getShape(int dx, int dy) const override;
};

#endif // ELLIPSEBRUSHTOOL_H
