
#include <QQueue>
#include <QPair>

#include "filltool.h"


// QPoints are not hashable in Qt by default!
inline uint qHash (const QPoint & key)
{
    return qHash (QPair<int,int>(key.x(), key.y()) );
}



FillTool::FillTool(TileMap *tileMap, SharedTileTemplate drawWith)
    : AbstractTileMapTool(tileMap),
      mDrawMaterial(drawWith)
{
}



void FillTool::cellClicked(int x, int y)
{

    // Update the fill selection.
    updateSelection(x, y);

    // Fill it in.
    SharedTileTemplate drawMaterial = getDrawMaterial();
    for (QPoint point : mSelection)
        mTileMap->setTile(point.x(), point.y(), drawMaterial);
}



void FillTool::updateSelection(int x, int y)
{

    QQueue<QPoint> toBeProcessed;
    QSet<QPoint> inQueueOrProcessed;

    toBeProcessed.enqueue(QPoint(x, y));

    while (!toBeProcessed.isEmpty()) {


        QPoint p = toBeProcessed.dequeue();


        qDebug() << "processing tile (" << p.x() << ", " << p.y() << ")";

        // Add the point to the selection.
        mSelection.insert(p);

        // The Tile corresponding to the current point.
        const Tile &pTile = mTileMap->tileAt(p.x(), p.y());

        // Add the point's neighbors to the queue if they match the point and
        // are not already in the queue.
        QPoint neighbors[8] = {
            QPoint(p.x() + 1, p.y() + 1),
            QPoint(p.x() + 0, p.y() + 1),
            QPoint(p.x() - 1, p.y() + 1),

            QPoint(p.x() + 1, p.y() + 0),
            QPoint(p.x() - 1, p.y() + 0),

            QPoint(p.x() + 1, p.y() - 1),
            QPoint(p.x() + 0, p.y() - 1),
            QPoint(p.x() - 1, p.y() - 1)

        };

        for (QPoint neighbor : neighbors) {

            // Only consider neighbors that aren't already in the queue and
            // weren't already processed.
            if (!inQueueOrProcessed.contains(neighbor)) {

                int nx = neighbor.x();
                int ny = neighbor.y();

                // Make sure neighbor is in bounds.
                if (nx >= 0 && nx < mTileMap->width()
                        && ny >= 0 && ny < mTileMap->height()) {


                    // Check if the neighbor matches the current point.
                    bool tileMatches = true;
                    if (mTileMap->tileAt(nx, ny).hasTileTemplate() != pTile.hasTileTemplate())
                        tileMatches = false;
                    else if (mTileMap->tileAt(nx, ny).tileTemplate() != pTile.tileTemplate())
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



SharedTileTemplate FillTool::getDrawMaterial() const
{
    // TODO: This will be changed in the future.
    return mDrawMaterial;
}
