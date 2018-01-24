#include "rectcell.h"
#include <QPainter>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

QBrush tempBrushColor;

rectCell::rectCell(qreal x,qreal y,qreal w, qreal h) :
    edited(false)
{ 
   this->setRect(x,y,w,h);
   this->setAcceptHoverEvents(true);
   this->setAcceptedMouseButtons(Qt::MouseButton(3));
   this->setBrush(Qt::white);
   QPen pen(Qt::black, 0, Qt::DashDotLine);
   this->setPen(pen);
}


void rectCell::mousePressEvent(QGraphicsSceneMouseEvent *e){

    if(e->button() == Qt::LeftButton){
        this->setBrush(QColor(108,147,209,255));
        tempBrushColor = this->brush();
        this->update();
    }
    //Right click to erase the color
    if(e->button() == Qt::RightButton){
        this->setBrush(Qt::white);
        tempBrushColor = this->brush();
        this->update();
    }
}

void rectCell::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
      tempBrushColor = this->brush();
      this->setBrush(Qt::gray);
      this->update();
}

void rectCell::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
       this->setBrush(tempBrushColor);
       this->update();
}
