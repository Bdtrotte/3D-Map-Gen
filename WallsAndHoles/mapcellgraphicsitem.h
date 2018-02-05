#ifndef MAPCELLGRAPHICSITEM_H
#define MAPCELLGRAPHICSITEM_H
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
    MapCellGraphicsItem(int x, int y, qreal w, qreal h);

    QRectF boundingRect() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setPen(QPen pen);
    void setBrush(QBrush brush);
    void setRect(int x, int y, qreal w, qreal h);
private:
    int mX;
    int mY;
    qreal mW;
    qreal mH;

    QBrush mBrush;
    QPen mPen;
};

#endif // MAPCELLGRAPHICSITEM_H
