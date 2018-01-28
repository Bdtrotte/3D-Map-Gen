#include "mapview.h"
#include <rectcell.h>
#include <QDebug>
#include <QScrollBar>

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent),
      mScale(0.5),
      mMapWidth(20),
      mMapHeight(20)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);
    for(qreal i = 0; i < mMapHeight; i++){
        for(qreal j = 0; j < mMapWidth; j++){
            RectCell *temp = new RectCell(j*30,i*30,30,30,i,j);
            connect(temp,SIGNAL(emitTool(int,int)),this,SLOT(receiveRectCell(int,int)));
           //fix tomorrow
            connect(std::bind(&MapView::emitRect, std::ref(i),std::ref(j))
                    ,temp, &RectCell::receiveTool );
            scene->addItem(temp);

        }
    }
    QMatrix mat;
    mat.scale(mScale,mScale);
    setMatrix(mat);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    //for future: make this tied to
    //the MapHeight and MapWidth numbers

    float d = event->delta();

    //on alt pan
    if (event->modifiers() & Qt::AltModifier) horizontalScrollBar()->setValue(horizontalScrollBar()->value() + d);
    //on ctrl zoom
    else if (event->modifiers() & Qt::ControlModifier) mScale += (d/1000);
    //otherwise normal scroll
    else QGraphicsView::wheelEvent(event);//else normal behavior

    if (mScale < 0.2) {
       //afformentioned basis changing code goes here
       mScale = 0.2;
    } else if (mScale > 5) {
       //afforemention basis changing code goes here
       mScale = 5;
    }

    QMatrix mat;
    mat.scale(mScale,mScale);
    setMatrix(mat);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MiddleButton) {
        int dx = mOldX - event->x();
        int dy = mOldY - event->y();
        mOldX = event->x();
        mOldY = event->y();
        QScrollBar* const hsb = horizontalScrollBar();
        hsb->setValue(hsb->value() + dx);
        QScrollBar* const vsb = verticalScrollBar();
        vsb->setValue(vsb->value() + dy);
    } else {
        QGraphicsView::mouseMoveEvent(event);
        mOldX = event->x();
        mOldY = event->y();
    }
}

void MapView::clear()
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);

    for(qreal i = 0; i < mMapHeight; i++){
        for(qreal j = 0; j < mMapWidth; j++){
            RectCell *temp = new RectCell(j*30,i*30,30,30,i,j);
            scene->addItem(temp);
        }
    }
}

void MapView::createMap(int tx, int ty)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);

    for(qreal i = 0; i < tx; i++){
        for(qreal j = 0; j < ty; j++){
            RectCell *temp = new RectCell(j*30,i*30,30,30,i,j);
            scene->addItem(temp);
        }
    }
}

void MapView::receiveRectCell(int i, int j){
    qDebug() << "test";
    emit emitRect(i,j);
}

void MapView::emitRect(int i, int j){

}
