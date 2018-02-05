#include "mapcellgraphicsitem.h"

MapCellGraphicsItem::MapCellGraphicsItem(int x, int y, qreal w, qreal h)
    :
    mX(x),
    mY(y),
    mW(w),
    mH(h)
{
}
void MapCellGraphicsItem::setBrush(QBrush brush){
    mBrush = brush;
}

void MapCellGraphicsItem::setPen(QPen pen){
    mPen = pen;
}

QRectF MapCellGraphicsItem::boundingRect() const
{
    return QRectF(mX, mY, mW, mH);
}

void MapCellGraphicsItem::setRect(int x, int y, qreal w, qreal h){
    mX = x;
    mY = y;
    mW = w;
    mH = h;
}

void MapCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    painter->setBrush(mBrush);
    painter->setPen(mPen);
    painter->drawRect(rect);
    update();
}

