#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "array2d.h"
#include "mapcell.h"
#include "tilemap.h"

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

    void clear();
    void createMap(TileMap *tileMap);

private slots:
    void setDefaultView();
    void setHeightMap();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    //emited when the left mouse button is pressed while the mouse is over a given cell (clicked, or dragged over)
    void cellActivated(int x, int y);

    //emited when a cell is clicked
    void cellClicked(int x, int y);

    //emited when the mouse is released over this cell
    void cellReleased(int x, int y);

private:
    float mScale;
    int mOldX;
    int mOldY;

    Array2D<MapCell *> mMapCells;

    QPoint mPreMousePoint;

    const QRegion &mSelectedRegion;

    QToolBar *mToolBar;

    void setupViewToolBar();




};

#endif // MAPVIEW_H
