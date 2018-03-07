#include "filltool.h"

#include "tilemaphelpers.h"
#include "tiletemplatechangecommand.h"

#include <QQueue>
#include <QPair>


FillTool::FillTool(TileMapPreviewGraphicsItem *previewItem, QUndoStack *undoStack, QObject *parent)
    : AbstractTileMapTool(previewItem, parent)
    , mUndoStack(undoStack) {}

void FillTool::cellClicked(int x, int y, QMouseEvent *)
{
    // Clear the overlay.
    clearOverlay();

    // Update the fill selection.
    updateSelection(x, y);


    // Temporarily block invalidateSelection() signals.
    disconnect(getTileMap(), &TileMap::mapChanged, this, &FillTool::invalidateSelection);

    // Fill it in.
    mUndoStack->push(TileTemplateChangeCommand::performCommand(
                         getTileMap(),
                         mSelection,
                         getTileTemplate(),
                         "'fill'"));

    // Stop blocking signals.
    connect(getTileMap(), &TileMap::mapChanged, this, &FillTool::invalidateSelection);
}


void FillTool::cellHovered(int x, int y, QMouseEvent *)
{
    drawOverlay(x, y);
}


void FillTool::toolTileMapChanged(TileMap *prev)
{
    // Clear mSelection because it is no longer valid.
    invalidateSelection();

    // Disconnect old connections.
    if (prev != nullptr)
        prev->disconnect(this);

    // Make new connections.
    if (getTileMap())
        connect(getTileMap(), &TileMap::mapChanged, this, &FillTool::invalidateSelection);
}


void FillTool::invalidateSelection()
{
    mSelection.clear();
}


void FillTool::updateSelection(int x, int y)
{
    if (mSelection.contains(QPoint(x, y)))
        return;

    mSelection.clear();

    QRegion fillRegion = TileMapHelper::getFillRegion(getTileMap(), x, y);

    for (const QRect &r : fillRegion) {
        for (int w = r.left(); w <= r.right(); ++w) {
            for (int h = r.top(); h <= r.bottom(); ++h) {
                mSelection.insert(QPoint(w, h));
            }
        }
    }
}



void FillTool::clearOverlay()
{
    mPreviewItem->setRegion(QRegion());
}


void FillTool::drawOverlay(int endX, int endY)
{
    if (!mSelection.contains(QPoint(endX, endY))) {
        clearOverlay();

        updateSelection(endX, endY);

        QRegion region;
        for (const QPoint &p : mSelection)
            region += QRect(p, QSize(1,1));

        mPreviewItem->setRegion(region);

        if (TileTemplate *t = getTileTemplate())
            mPreviewItem->setColor(t->color());
        else
            mPreviewItem->setColor(Qt::gray);
    }
}

void FillTool::mouseExitedMap(QMouseEvent *)
{
    mSelection.clear();
    clearOverlay();
}

void FillTool::deactivate()
{
    mSelection.clear();
    clearOverlay();
}
