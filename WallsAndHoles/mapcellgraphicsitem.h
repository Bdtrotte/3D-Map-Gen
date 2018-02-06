#ifndef MAPCELLGRAPHICSITEM_H
#define MAPCELLGRAPHICSITEM_H
#include "tile.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

enum MapViewMode {
    NoView = 0,
    DefaultView = 1,
    HeightMapView = 2
};

/**
 * @brief The MapCellGraphicsItem class
 * Inherits from QGraphicsItem this class
 * is used for painting and generating the cells in MapCell
 */
class MapCellGraphicsItem : public QGraphicsItem
{
public:
    MapCellGraphicsItem(int x, int y, qreal w, qreal h, const Tile &tile);

    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *,
               QWidget *);

    void setViewMode(MapViewMode viewMode, bool enabled);
private:
    const Tile &mTile;

    int mViewMode;

    QRectF mRect;
};

#endif // MAPCELLGRAPHICSITEM_H
