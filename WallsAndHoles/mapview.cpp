#include "mapview.h"
#include <QGraphicsView>
#include <rectcell.h>
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>

MapView::MapView(QWidget *parent)
: QGraphicsView(parent),
  mScale(0.5),
  mMapWidth(20),
  mMapHeight(20),
  mKeyDown(Qt::NoModifier)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);

    for(qreal i = 0; i < this->mMapHeight*30; i+=30){
        for(qreal j = 0; j < this->mMapWidth*30; j+=30){
            RectCell *temp = new RectCell(j+100,i+100,30,30);
            scene->addItem(temp);
        }
    }
    QMatrix mat;
    mat.scale(mScale,mScale);
    setMatrix(mat);

}


void MapView::keyPressEvent(QKeyEvent *event){
    qDebug() << event->modifiers();
    mKeyDown = event->modifiers();
    QGraphicsView::keyPressEvent(event);
}

void MapView::keyReleaseEvent(QKeyEvent *event){
     mKeyDown = Qt::NoModifier;
     QGraphicsView::keyReleaseEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    //for future: make this tied to
    //the MapHeight and MapWidth numbers

  float d = event->delta();

  //on CTRL pan
  if(mKeyDown==Qt::MetaModifier) {QGraphicsView::wheelEvent(event);};

  //on Option zoom
  if(mKeyDown==Qt::AltModifier) {mScale += (d/1000);};

   if(mScale < 0.2){
       //afformentioned basis changing code goes here
       mScale = 0.2;
   }else if(mScale > 5){
       //afforemention basis changing code goes here
       mScale = 5;
   }

   QMatrix mat;
   mat.scale(mScale,mScale);
   setMatrix(mat);

}

void MapView::clear(){
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    this->setScene(scene);
        for(qreal i = 0; i < this->mMapHeight*30; i+=30){
            for(qreal j = 0; j < this->mMapWidth*30; j+=30){
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



