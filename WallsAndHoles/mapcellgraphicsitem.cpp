#include "mapcellgraphicsitem.h"

MapCellGraphicsItem::MapCellGraphicsItem(int x, int y, qreal w, qreal h)
    :
    QWidget(parent),
    mX(x),
    mY(y),
    mW(w),
    mH(h)

{

}


QRectF MapCellGraphicsItem::boundingRect() const
{
    return QRectF(mX, mY, mW, mH);
}

void MapCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *parent)
{
    QRect rect = boundingRect();
}
