#ifndef FILLTOOL_H
#define FILLTOOL_H


#include <QSet>
#include <QPoint>

#include "abstracttilemaptool.h"

class FillTool : public QObject, public AbstractTileMapTool {
    Q_OBJECT

public:
    FillTool(TileMap *tileMap);

    /**
     * @brief This will immediately fill an area.
     * @param x  The X position of the clicked tile.
     * @param y  The Y position of the clicked tile.
     */
    void cellClicked(int x, int y) override;


    void toolTileMapChanged(TileMap *prev) override;

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

};

#endif // FILLTOOL_H
