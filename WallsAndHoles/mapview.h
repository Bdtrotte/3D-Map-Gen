#ifndef MAPVIEW_H
#define MAPVIEW_H

#define MAP_BUFFER 5 //Tiles kept around the map for the purpose of scrolling.

#include "array2d.h"
#include "mapcell.h"
#include "tilemap.h"
#include "tilemappreviewgraphicsitem.h"

#include <QObject>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QResizeEvent>

/**
 * @brief The MapView class
 * Handles display of TileMap
 */
class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    MapView(QWidget *parent);
    ~MapView();

    void clear();
    void setMap(TileMap *tileMap);

    void setViewMode(int viewMode);

    TileMapPreviewGraphicsItem *previewItem() { return mPreviewItem; }

    /**
     * @brief tilesInFrame
     *
     * The corners of the returned rect are the tile cords under the
     * corners of the frame of this widget.
     *
     * @return
     */
    QRectF tilesInFrame() const;

private slots:
    void mapSizeChanged();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *) override;

signals:
    /**
     * @brief Emitted when the left mouse button is down over a new cell.
     *
     * This signal is sent out when the mouse button is pressed down or when the mouse
     * moves over a new cell while pressed down.
     *
     * x and y are NOT required to be contained within the map!
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellActivated(int x, int y, QMouseEvent *event);

    /**
     * @brief Emitted the first time the left mouse button is pressed over a cell.
     *
     * Unlike cellActivated(), this signal is sent out when the mouse button is first pressed
     * and not sent out for other cells if the mouse is dragged afterward.
     *
     * x and y are NOT required to be contained within the map!
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellClicked(int x, int y, QMouseEvent *event);

    /**
     * @brief Emitted when the left mouse button is released over a cell.
     *
     * x and y are NOT required to be contained within the map!
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellReleased(int x, int y, QMouseEvent *event);


    /**
     * @brief Emitted when the mouse hovers over a new cell.
     *
     *
     * x and y ARE required to be contained within the map!
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellHovered(int x, int y, QMouseEvent *event);


    /**
     * @brief Emitted when the mouse exits the map.
     */
    void mouseExitedMap(QMouseEvent *event);

    /**
     * @brief mapViewChanged
     *
     * emitted whenever the map changes position relative the the frame of this widget.
     *
     * @param rect
     *
     * The corners of this rect are the tile cords under the corners of the widget
     * Wont necessarily be in the bounds of the map.
     */
    void mapViewChanged(QRectF rect);

private:
    /**
     * @brief reMakeMap
     * Deletes the old map, and if mTileMap isn't null,
     * makes the map.
     */
    void reMakeMap();

    float mScale;
    int mOldX;
    int mOldY;

    const TileMap *mTileMap;

    Array2D<MapCell *> mMapCells;
    int mViewMode;

    QGraphicsRectItem *mMouseHoverRect;

    //this owns mPreviewItem
    TileMapPreviewGraphicsItem *const mPreviewItem;

    QPoint mPreMousePoint;
};

#endif // MAPVIEW_H
