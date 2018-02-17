#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "array2d.h"
#include "mapcell.h"
#include "tilemap.h"
#include "tilemappreviewgraphicsitem.h"

#include <QObject>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWidget>
#include <QRegion>
#include <QToolBar>

/**
 * @brief The MapView class
 * Handles display of TileMap
 */
class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    MapView(const QRegion &selectedRegion, QWidget *parent);
    ~MapView();

    void clear();
    void createMap(TileMap *tileMap);

    TileMapPreviewGraphicsItem *previewItem() { return mPreviewItem; }

private slots:
    void setNoView(bool state);
    void setDefaultView(bool state);
    void setHeightMap(bool state);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    /**
     * @brief Emitted when the left mouse button is down over a new cell.
     *
     * This signal is sent out when the mouse button is pressed down or when the mouse
     * moves over a new cell while pressed down.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellActivated(int x, int y);

    /**
     * @brief Emitted the first time the left mouse button is pressed over a cell.
     *
     * Unlike cellActivated(), this signal is sent out when the mouse button is first pressed
     * and not sent out for other cells if the mouse is dragged afterward.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellClicked(int x, int y);

    /**
     * @brief Emitted when the left mouse button is released over a cell.
     *
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellReleased(int x, int y);


    /**
     * @brief Emitted when the mouse hovers over a new cell.
     * @param x The cell's x position.
     * @param y The cell's y position.
     */
    void cellHovered(int x, int y);


    /**
     * @brief Emitted when the mouse exits the map.
     */
    void mouseExitedMap();

private:
    void setupViewToolBar();

    float mScale;
    int mOldX;
    int mOldY;

    Array2D<MapCell *> mMapCells;

    //this owns mPreviewItem
    TileMapPreviewGraphicsItem *const mPreviewItem;

    QPoint mPreMousePoint;

    const QRegion &mSelectedRegion;

    QToolBar *mToolBar;
    QAction *mNoView;
    QAction *mDefaultView;
    QAction *mHeightView;
};

#endif // MAPVIEW_H
