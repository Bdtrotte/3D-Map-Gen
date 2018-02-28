#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(TileMapPreviewGraphicsItem *previewItem, QObject *parent)
    : AbstractTileMapTool(previewItem, parent) {}

void AbstractShapeBrushTool::cellClicked(int x, int y, QMouseEvent *)
{
    mStartX = x;
    mStartY = y;

    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellActivated(int x, int y, QMouseEvent *)
{
    drawOverlay(x, y);
}


void AbstractShapeBrushTool::cellReleased(int x, int y, QMouseEvent *)
{
    clearOverlay();
    placeShape(x, y);
}

void AbstractShapeBrushTool::deactivate()
{
    clearOverlay();
}


void AbstractShapeBrushTool::drawOverlay(int endX, int endY) {
    clearOverlay();

    QVector<QPoint> shape = getShape(endX - mStartX, endY - mStartY);

    QRegion region;

    for (const QPoint &p : shape)
        region += QRect(p.x() + mStartX, p.y() + mStartY, 1, 1);

    mPreviewItem->setRegion(region);
    if (TileTemplate *t = getTileTemplate())
        mPreviewItem->setColor(t->color());
    else
        mPreviewItem->setColor(Qt::gray);
}

void AbstractShapeBrushTool::clearOverlay() {
    // Clears overlay.
    mPreviewItem->setRegion(QRegion());
}

void AbstractShapeBrushTool::placeShape(int endX, int endY) {
    QVector<QPoint> shape = getShape(endX - mStartX, endY - mStartY);

    for (QPoint p : shape) {
        int x = mStartX + p.x();
        int y = mStartY + p.y();

        if (x >= 0 && x < getTileMap()->width() && y >= 0 && y < getTileMap()->height())
            getTileMap()->setTile(x, y, getTileTemplate());
    }
}
