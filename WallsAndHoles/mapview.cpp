#include "mapview.h"
#include <QGraphicsView>
#include <rectcell.h>
#include <QDebug>
#include <QScrollBar>

MapView::MapView(QWidget *parent)
: QGraphicsView(parent),
  scale(0.5),
  oldX(0),
  oldY(0),
  MapWidth(20),
  MapHeight(20)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(qreal i = 0; i < this->MapHeight*30; i+=30){
        for(qreal j = 0; j < this->MapWidth*30; j+=30){
            RectCell *temp = new RectCell(j+100,i+100,30,30);
            scene->addItem(temp);
        }
    }
    QMatrix mat;
    mat.scale(this->scale,this->scale);
    this->setMatrix(mat);

}


void MapView::wheelEvent(QWheelEvent *event)
{
   float d = event->delta();
   this->scale += (d/1000);
   if(this->scale < 0.2){
       this->scale = 0.2;
   }else if(this->scale > 5){
       this->scale = 5;
   }
   QMatrix mat;
   mat.scale(this->scale,this->scale);
   this->setMatrix(mat);

}

void MapView::clear(){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


        for(qreal i = 0; i < this->MapHeight*30; i+=30){
            for(qreal j = 0; j < this->MapWidth*30; j+=30){
                RectCell *temp = new RectCell(j+100,i+100,30,30);
                scene->addItem(temp);
            }
        }
}

void MapView::createMap(int tx, int ty){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        for(qreal i = 0; i < tx*30; i+=30){
            for(qreal j = 0; j < ty*30; j+=30){
                RectCell *temp = new RectCell(j+100,i+100,30,30);
                scene->addItem(temp);
            }
        }
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{

    if( event->buttons() == Qt::MiddleButton){
       int dx = this->oldX - event->x();
       int dy = this->oldY - event->y();
       this->oldX = event->x();
       this->oldY = event->y();
       QScrollBar* const hsb = this->horizontalScrollBar();
       hsb->setRange(0, 1000);
       hsb->setValue(hsb->value() + dx);
       QScrollBar* const vsb = this->verticalScrollBar();
       vsb->setRange(0, 1000);
       vsb->setValue(vsb->value() + dy);
    }else{
    QGraphicsView::mouseMoveEvent(event);
     this->oldX = event->x();
     this->oldY = event->y();
    }
}

