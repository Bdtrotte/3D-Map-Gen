#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractTileMapTool(tileMap),
      mDrawMaterial(drawWith)
{

}


void AbstractShapeBrushTool::cellClicked(int x, int y) {
    mStartX = x;
    mStartY = y;

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
    // TODO: Draw an overlay using shape.

    qDebug() << "Shape overlay from (" << mStartX << ", " << mStartY << ") to (" << endX << ", " << endY << ")";
}

void AbstractShapeBrushTool::clearOverlay() {
    // TODO: Clear overlay.
}

void AbstractShapeBrushTool::placeShape(int endX, int endY) {
    Array2D<bool> shape = getShape(endX - mStartX, endY - mStartY);

    for (int dx = 0; dx < shape.width(); ++dx)
        for (int dy = 0; dy < shape.height(); ++dy)
            if (shape(dx, dy))
                mTileMap->setTile(mStartX + dx, mStartY + dy, mDrawMaterial);
}
