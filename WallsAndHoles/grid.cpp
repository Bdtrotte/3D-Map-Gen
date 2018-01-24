#include "grid.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QApplication>
#include "stdlib.h"

Grid::Grid(QWidget *parent):
    mouseDown(false),
    prevX(0),
    prevY(0)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//void Grid::mousePressEvent(QMouseEvent *e)
//{

// QGraphicsSceneMouseEvent *q;
// qDebug() << e->pos();

//}

//void Grid::wheelEvent(QWheelEvent *event)
//{

//}

//void Grid::mouseReleaseEvent(QMouseEvent *e)
//{
//    e->ignore();
////    if(this->mouseDown){
////        qDebug() << "test";
////    }else{
////        e->ignore();
////    }


//}

//void Grid::mouseMoveEvent(QMouseEvent *e){
//    e->ignore();
//    if(this->mouseDown){
//        qDebug() << "move event";
////    }else{
////        e->ignore();
//    }
//}
