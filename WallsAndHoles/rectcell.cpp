#include "rectcell.h"
#include <QPainter>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

rectCell::rectCell(qreal x,qreal y,qreal w, qreal h)
{ 
   this->setRect(x,y,w,h);
   this->setAcceptHoverEvents(true);
   this->setAcceptedMouseButtons(Qt::MouseButton(1));
}


void rectCell::mousePressEvent(QGraphicsSceneMouseEvent *e){
    qDebug() << "click";
}

void rectCell::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
   qDebug() << "hover";
}
