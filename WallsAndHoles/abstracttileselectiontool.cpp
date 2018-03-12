#include "abstracttileselectiontool.h"

#include "tilepropertymanager.h"

QRegion AbstractTileSelectionTool::mSelection;

AbstractTileSelectionTool::AbstractTileSelectionTool(PropertyBrowser *propertyBrowser,
                                                     TileMapPreviewGraphicsItem *previewItem)
    : AbstractTileMapTool(previewItem)
    , mPropertyBrowser(propertyBrowser) {}

void AbstractTileSelectionTool::deactivate()
{
    mSelection = QRegion();
    mPreviewItem->setRegion(QRegion());
    mPropertyBrowser->clear();
}

void AbstractTileSelectionTool::activateSelection()
{
    drawPreview(mSelection);

    if (!mSelection.isEmpty()) {
        QVector<Tile *> tiles;

        TileMap *tileMap = getTileMap();

        for (const QRect &r : mSelection) {
            for (int x = r.left(); x <= r.right(); ++x) {
                for (int y = r.top(); y <= r.bottom(); ++y) {
                    Tile *t = &tileMap->tileAt(x, y);
                    if (t->hasTileTemplate())
                        tiles.append(t);
                }
            }
        }

        if (tiles.isEmpty()) {
            mPropertyBrowser->clear();
            return;
        }

        mPropertyBrowser->setPropertyManager(new TilePropertyManager(tiles));
    } else {
        mPropertyBrowser->clear();
    }
}

void AbstractTileSelectionTool::drawPreview(QRegion previewRegion)
{
    mPreviewItem->setRegion(previewRegion);
    mPreviewItem->setColor(QColor(200, 200, 255, 100));
}
