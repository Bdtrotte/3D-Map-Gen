#ifndef RECTBRUSHTOOL_H
#define RECTBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class RectBrushTool : public AbstractShapeBrushTool
{
public:
    RectBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack);

    /// @brief Draws a rectangle.
   QRegion getShape(QPoint start, QPoint end) const override;
};

#endif // RECTBRUSHTOOL_H
