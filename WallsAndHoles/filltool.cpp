
#include <QQueue>
#include <QPair>

#include "filltool.h"


// QPoints are not hashable in Qt by default!
inline uint qHash (const QPoint & key)
{
    return qHash (QPair<int,int>(key.x(), key.y()) );
}



FillTool::FillTool(TileMapPreviewGraphicsItem *previewItem)
    : AbstractTileMapTool(previewItem) {}

void FillTool::cellClicked(int x, int y)
{
    // Clear the overlay.
    clearOverlay();

    // Update the fill selection.
    updateSelection(x, y);


    // Temporarily block invalidateSelection() signals.
    disconnect(getTileMap(), &TileMap::mapChanged, this, &FillTool::invalidateSelection);

    // Fill it in.
    TileTemplate *drawMaterial = getTileTemplate();
    for (QPoint point : mSelection)
        getTileMap()->setTile(point.x(), point.y(), drawMaterial);

    // Stop blocking signals.
    connect(getTileMap(), &TileMap::mapChanged, this, &FillTool::invalidateSelection);
}


void FillTool::cellHovered(int x, int y)
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
    QQueue<QPoint> toBeProcessed;
    QSet<QPoint> inQueueOrProcessed;

    toBeProcessed.enqueue(QPoint(x, y));

    while (!toBeProcessed.isEmpty()) {

        QPoint p = toBeProcessed.dequeue();


        // Add the point to the selection.
        mSelection.insert(p);

        // The Tile corresponding to the current point.
        const Tile &pTile = getTileMap()->tileAt(p.x(), p.y());

        // Add the point's neighbors to the queue if they match the point and
        // are not already in the queue.
        QPoint neighbors[4] = {
            QPoint(p.x() + 0, p.y() + 1),

            QPoint(p.x() + 1, p.y() + 0),
            QPoint(p.x() - 1, p.y() + 0),

            QPoint(p.x() + 0, p.y() - 1)

        };

        for (QPoint neighbor : neighbors) {

            // Only consider neighbors that aren't already in the queue and
            // weren't already processed.
            if (!inQueueOrProcessed.contains(neighbor)) {

                int nx = neighbor.x();
                int ny = neighbor.y();

                // Make sure neighbor is in bounds.
                if (nx >= 0 && nx < getTileMap()->width()
                        && ny >= 0 && ny < getTileMap()->height()) {


                    // Check if the neighbor matches the current point.
                    bool tileMatches = true;
                    if (getTileMap()->tileAt(nx, ny).hasTileTemplate() != pTile.hasTileTemplate())
                        tileMatches = false;
                    else if (getTileMap()->tileAt(nx, ny).tileTemplate() != pTile.tileTemplate())
                        tileMatches = false;


                    // If it does match, then add it to the processing queue.
                    if (tileMatches) {
                        toBeProcessed.enqueue(neighbor);
                        inQueueOrProcessed.insert(neighbor);
                    }
                }
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

void FillTool::mouseExitedMap()
{
    mSelection.clear();
    clearOverlay();
}

void FillTool::deactivate()
{
    mSelection.clear();
    clearOverlay();
}
