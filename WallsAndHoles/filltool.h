#ifndef FILLTOOL_H
#define FILLTOOL_H


#include <QSet>
#include <QPoint>

#include "abstracttilemaptool.h"
#include "mapview.h"
#include "mapoverlaycell.h"

class FillTool : public QObject, public AbstractTileMapTool {
    Q_OBJECT

public:
    FillTool(MapView *mapView, TileMap *tileMap);

    /**
     * @brief This will immediately fill an area.
     * @param x  The X position of the clicked tile.
     * @param y  The Y position of the clicked tile.
     */
    void cellClicked(int x, int y) override;


    /**
     * @brief This will show a preview for the selected area.
     */
    void cellHovered(int x, int y) override;

    void toolTileMapChanged(TileMap *prev) override;


    void mouseExitedMap() override;
    void deactivate() override;

    /* TODO:
     *
     * There should be a fill tool preview.
     *
     * There should be a "cellHovered" signal in MapView that will
     * update the fill preview.
     * */

public slots:

    void invalidateSelection();

protected:


    /// Updates mSelection to match the points that will be filled if
    /// the given point is selected.
    void updateSelection(int x, int y);


    /// The set of points that will be filled in.
    QSet<QPoint> mSelection;






    /// The MapView on which an overlay will be drawn.
    MapView *mMapView;

    /// The overlay that is drawn over the map view.
    Array2D<QSharedPointer<MapOverlayCell>> mOverlay;

    /// Draws an overlay previewing the area that will be filled.
    void drawOverlay(int endX, int endY);

    /// Clears the overlay.
    void clearOverlay();
};

#endif // FILLTOOL_H
