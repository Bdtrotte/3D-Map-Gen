#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(MapView *mapView, TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractTileMapTool(tileMap),
      mMapView(mapView),
      mDrawMaterial(drawWith)
{

}


void AbstractShapeBrushTool::cellClicked(int x, int y) {
    mStartX = x;
    mStartY = y;

    Q_ASSERT( mStartX >= 0 && mStartX < mTileMap->width() );
    Q_ASSERT( mStartY >= 0 && mStartY < mTileMap->height() );

    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellActivated(int x, int y) {
    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellReleased(int x, int y) {
    clearOverlay();
    placeShape(x, y);
}



void AbstractShapeBrushTool::drawOverlay(int endX, int endY) {
    clearOverlay();

    Array2D<bool> shape = getShape(endX - mStartX, endY - mStartY);

    int startX = std::min(mStartX, endX);
    int startY = std::min(mStartY, endY);

    mOverlay = Array2D<QSharedPointer<MapOverlayCell>>(mTileMap->mapSize());

    QGraphicsScene *scene = mMapView->scene();

    for (int dx = 0; dx < shape.width(); ++dx)
        for (int dy = 0; dy < shape.height(); ++dy)
            if (shape(dx, dy))
                mOverlay(startX + dx, startY + dy) = QSharedPointer<MapOverlayCell>::create(scene, startX + dx, startY + dy, QColor(255, 0, 0, 100));
}

void AbstractShapeBrushTool::clearOverlay() {
    // Clears overlay.
    mOverlay = Array2D<QSharedPointer<MapOverlayCell>>();
}

void AbstractShapeBrushTool::placeShape(int endX, int endY) {
    Array2D<bool> shape = getShape(endX - mStartX, endY - mStartY);

    Q_ASSERT( endX >= 0 && endX < mTileMap->width() );
    Q_ASSERT( endY >= 0 && endY < mTileMap->height() );

    int startX = std::min(mStartX, endX);
    int startY = std::min(mStartY, endY);

    for (int dx = 0; dx < shape.width(); ++dx)
        for (int dy = 0; dy < shape.height(); ++dy)
            if (shape(dx, dy))
                mTileMap->setTile(startX + dx, startY + dy, mDrawMaterial);
}
