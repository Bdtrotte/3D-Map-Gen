#ifndef ABSTRACTSHAPEBRUSHTOOL_H
#define ABSTRACTSHAPEBRUSHTOOL_H

#include <QVector>
#include <QPoint>

#include "array2d.h"
#include "abstracttilemaptool.h"
#include "tilemap.h"
#include "tiletemplate.h"
#include "mapview.h"

class AbstractShapeBrushTool : public AbstractTileMapTool
{
public:
    AbstractShapeBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent = nullptr);

    void cellClicked(int x, int y, QMouseEvent *) override;
    void cellActivated(int x, int y, QMouseEvent *) override;
    void cellReleased(int x, int y, QMouseEvent *) override;

    void deactivate() override;

    /**
     * @brief This function should output a QVector of points that should be filled in.
     *
     * All points are specified as offsets from (0, 0), which is assumed to be the place
     * where the mouse drag started. The point (dx, dy) is the place where the mouse drag
     * ended.
     *
     * There is no assumption on whether the points lie in the rectangle between (0, 0) and (dx, dy).
     *
     * @param dx  The X offset from the start of drawing.
     * @param dy  The Y offset from the start of drawing.
     */
    virtual QVector<QPoint> getShape(int dx, int dy) const = 0;

private:
    int mStartX;  /// The X position of the first click.
    int mStartY;  /// The Y position of the first click.

    /// The MapView on which an overlay will be drawn.
    MapView *mMapView;

    /// Draws an overlay previewing the shape that will be drawn.
    void drawOverlay(int endX, int endY);

    /// Clears the overlay.
    void clearOverlay();

    /// Places down a shape.
    void placeShape(int endX, int endY);
};

#endif // ABSTRACTSHAPEBRUSHTOOL_H
