#include "rectcell.h"
#include <QPainter>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

QBrush tempBrushColor;

RectCell::RectCell(qreal x,qreal y,qreal w, qreal h) :
    mEdited(false)
{ 
   setRect(x,y,w,h);
   setAcceptHoverEvents(true);
   setAcceptedMouseButtons(Qt::MouseButton(3));
   setBrush(Qt::white);
   QPen pen(Qt::black, 0, Qt::DashLine);
   setPen(pen);
}

void RectCell::applyTool(QString temptoolname)
{
    mEdited = !mEdited;
    emit toolSignal();
}


void RectCell::mousePressEvent(QGraphicsSceneMouseEvent *e){

    if(e->button() == Qt::LeftButton){
        setBrush(QColor(108,147,209,255));
        tempBrushColor = brush();
        update();
    }
    //Right click to erase the color
    if(e->button() == Qt::RightButton){
        setBrush(Qt::white);
        tempBrushColor = brush();
        update();
    }
}

void RectCell::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
      tempBrushColor = brush();
      setBrush(Qt::gray);
      update();
}

void RectCell::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
       setBrush(tempBrushColor);
       update();
}

void RectCell::toolSignal(){

}
