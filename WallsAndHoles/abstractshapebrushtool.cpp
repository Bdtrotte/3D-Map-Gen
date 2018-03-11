#include "abstractshapebrushtool.h"

AbstractShapeBrushTool::AbstractShapeBrushTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractTileMapTool(previewItem, parent)
    , mUndoStack(undoStack) {}

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

    QRegion region = getShape(QPoint(mStartX, mStartY), QPoint(endX, endY));

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
    QRegion region = getShape(QPoint(mStartX, mStartY), QPoint(endX, endY));

    mUndoStack->push(TileTemplateChangeCommand::make(
                         getTileMap(),
                         region,
                         getTileTemplate(),
                         "'draw shape'"));
}
