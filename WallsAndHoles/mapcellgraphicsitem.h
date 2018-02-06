#ifndef MAPCELLGRAPHICSITEM_H
#define MAPCELLGRAPHICSITEM_H
#include "tile.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

/**
 * @brief The MapCellGraphicsItem class
 * Inherits from QGraphicsItem this class
 * is used for painting and generating the cells in MapCell
 */
class MapCellGraphicsItem :public QGraphicsItem
{
public:
    enum Views{
        Noview = 0,
        DefaultView = 1,
        HeightMap = 2
    };
    MapCellGraphicsItem(int x, int y, qreal w, qreal h, const Tile &tile);

    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setPen(QPen pen);
    void setBrush(QBrush brush);
    void setRect(int x, int y, qreal w, qreal h);
    void setView(int viewFlag);
private:
    int mX;
    int mY;
    qreal mW;
    qreal mH;

    const Tile &mTile;

    int mViewFlag;

    QBrush mBrush;
    QPen mPen;

    QRectF mRect;


};

#endif // MAPCELLGRAPHICSITEM_H
