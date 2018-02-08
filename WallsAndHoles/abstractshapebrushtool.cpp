#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(MapView *mapView, TileMap *tileMap)
    : AbstractTileMapTool(tileMap),
      mMapView(mapView) {}


void AbstractShapeBrushTool::cellClicked(int x, int y) {
    mStartX = x;
    mStartY = y;

    Q_ASSERT( mStartX >= 0 && mStartX < getTileMap()->width() );
    Q_ASSERT( mStartY >= 0 && mStartY < getTileMap()->height() );

    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellActivated(int x, int y)
{
    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellReleased(int x, int y)
{
    clearOverlay();
    placeShape(x, y);
}


void AbstractShapeBrushTool::drawOverlay(int endX, int endY) {
    clearOverlay();

    QVector<QPoint> shape = getShape(endX - mStartX, endY - mStartY);

    mOverlay = Array2D<QSharedPointer<MapOverlayCell>>(getTileMap()->mapSize());

    QGraphicsScene *scene = mMapView->scene();

    QPoint start(mStartX, mStartY);
    for (QPoint p : shape)
        if (mOverlay.isInBounds(start + p))
            mOverlay(start + p) = QSharedPointer<MapOverlayCell>::create(scene, mStartX + p.x(), mStartY + p.y(), QColor(255, 0, 0, 100));
}

void AbstractShapeBrushTool::clearOverlay() {
    // Clears overlay.
    mOverlay = Array2D<QSharedPointer<MapOverlayCell>>();
}

void AbstractShapeBrushTool::placeShape(int endX, int endY) {
    QVector<QPoint> shape = getShape(endX - mStartX, endY - mStartY);

    Q_ASSERT( endX >= 0 && endX < getTileMap()->width() );
    Q_ASSERT( endY >= 0 && endY < getTileMap()->height() );

    for (QPoint p : shape) {
        int x = mStartX + p.x();
        int y = mStartY + p.y();

        if (x >= 0 && x < getTileMap()->width() && y >= 0 && y < getTileMap()->height())
            getTileMap()->setTile(x, y, getTileTemplate());
    }
}
