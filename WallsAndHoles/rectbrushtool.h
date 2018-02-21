#ifndef RECTBRUSHTOOL_H
#define RECTBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class RectBrushTool : public AbstractShapeBrushTool
{
public:
    RectBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent = nullptr);

    /// @brief Draws a rectangle.
    QVector<QPoint> getShape(int dx, int dy) const override;
};

#endif // RECTBRUSHTOOL_H
