#include "grid.h"
#include <QDebug>
#include <QMouseEvent>
#include "stdlib.h"

Grid::Grid(QWidget *parent):
    mouseDown(false),
    prevX(0),
    prevY(0)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//void Grid::mouseMoveEvent(QMouseEvent *e)
//{
////    if(mouseDown && e->button() == 4){
////        qDebug() << e->pos();
////        int dx = abs(prevX - e->x());
////        int dy = abs(prevY - e->y());
////        QMatrix mat;
////        mat.translate(dx,dy);
////        this->setMatrix(mat);
////        this->prevX = e->x();
////        this->prevY = e->y();
////    }
//}

//void Grid::mouseReleaseEvent(QMouseEvent *e)
//{
//   this->mouseDown = false;
//}

//void Grid::mousePressEvent(QMouseEvent *e)
//{
//    this->mouseDown = true;
//    if(this->prevX == 0 && this->prevY == 0){
//        this->prevX = e->x();
//        this->prevY = e->y();
//    }
//    qDebug() << this->prevX;
//}

