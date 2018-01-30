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
      mMapHeight(20)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);

    for(qreal i = 0; i < mMapHeight*30; i+=30){
        for(qreal j = 0; j < mMapWidth*30; j+=30){
            RectCell *temp = new RectCell(j+100,i+100,30,30);
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

void MapView::clear()
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);

    for (qreal i = 0; i < mMapHeight*30; i+=30) {
        for (qreal j = 0; j < mMapWidth*30; j+=30) {
            RectCell *temp = new RectCell(j+100,i+100,30,30);
            scene->addItem(temp);
        }
    }
}

void MapView::createMap(int tx, int ty)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);

    for(qreal i = 0; i < tx*30; i+=30) {
        for(qreal j = 0; j < ty*30; j+=30) {
            RectCell *temp = new RectCell(j+100,i+100,30,30);
            scene->addItem(temp);
        }
    }
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

void MapView::openMap(QString path){
    qDebug() << "openMap called";
}

void MapView::saveMap(QString path){
    QSize mapSize(10,10);
    //qDebug() << "creating sample map";
    //TileMap testMap(mapSize);
    qDebug() << "creating sample templateSet";
    TileTemplateSet testSet;
    qDebug() << "creating sample tempalte";
    SharedTileTemplate testTemp=SharedTileTemplate::create(10,10,QVector2D(0,0),Qt::blue);
    qDebug() << "add template to map and set";
    //testMap.setTile(0,0,testTemp);
    testSet.addTileTemplate(testTemp);
    XMLTool xml(path);
    qDebug() << "saving...";
    //xml.saveTileMap(testMap, testSet);
    xml.saveTileTemplateSet(testSet);
}
