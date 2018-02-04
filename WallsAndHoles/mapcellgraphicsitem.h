#ifndef MAPCELLGRAPHICSITEM_H
#define MAPCELLGRAPHICSITEM_H
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

class MapCellGraphicsItem : QGraphicsItem
{
public:
    MapCellGraphicsItem(int x, int y, qreal w, qreal h);

    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

private:
    int mX;
    int mY;
    qreal mW;
    qreal mH;
};

#endif // MAPCELLGRAPHICSITEM_H
