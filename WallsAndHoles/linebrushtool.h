#ifndef LINEBRUSHTOOL_H
#define LINEBRUSHTOOL_H


#include "abstractshapebrushtool.h"

class LineBrushTool : public AbstractShapeBrushTool {
public:
    LineBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawMaterial);

    /**
     * @brief Makes a 1-pixel line by intersecting a line with a grid.
     *
     * Note: this strategy produces "thick" looking lines usually. There is
     * another way that could be implemented that produces "thin" lines.
     */
    QVector<QPoint>getShape(int dx, int dy) const override;
};

#endif // LINEBRUSHTOOL_H
