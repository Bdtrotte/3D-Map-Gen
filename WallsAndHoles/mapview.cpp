#include "mapview.h"
#include <QGraphicsView>
#include <rectcell.h>
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>

MapView::MapView(QWidget *parent)
: QGraphicsView(parent),
  scale(0.5),
  MapWidth(20),
  MapHeight(20),
  KeyDown(-1)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);

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


void MapView::keyPressEvent(QKeyEvent *event){
    KeyDown = event->key();
}

void MapView::keyReleaseEvent(QKeyEvent *event){
     KeyDown = -1;
}

void MapView::wheelEvent(QWheelEvent *event)
{
    //for future: make this tied to
    //the MapHeight and MapWidth numbers

   float d = event->delta();

  if(KeyDown==16777251) {QGraphicsView::wheelEvent(event);};

  if(KeyDown==16777250) {scale += (d/1000);};
   if(scale < 0.2){
       scale = 0.2;
   }else if(scale > 5){
       scale = 5;
   }
   QMatrix mat;
   mat.scale(this->scale,this->scale);
   setMatrix(mat);

}

void MapView::clear(){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);
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

        for(qreal i = 0; i < tx*30; i+=30){
            for(qreal j = 0; j < ty*30; j+=30){
                RectCell *temp = new RectCell(j+100,i+100,30,30);
                scene->addItem(temp);
            }
        }
}



