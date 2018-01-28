#include "rectcell.h"
#include <QDebug>
RectCell::RectCell(qreal x, qreal y, qreal w, qreal h, qreal i, qreal j, QObject *parent) :
     mToolName("none"),
     mCoords(x,y),
     mIndices(i,j),
     mDimensions(w,h),
     mBrush(Qt::white),
     mPen(Qt::black, 0, Qt::DashLine)
{
 setAcceptHoverEvents(true);
}

void RectCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(mBrush);
    painter->setPen(mPen);
    painter->drawRect(std::get<0>(mCoords),std::get<1>(mCoords),
                     std::get<0>(mDimensions),std::get<1>(mDimensions));
}

QRectF RectCell::boundingRect() const{
    QRectF rect = QRectF(std::get<0>(mCoords),std::get<1>(mCoords),
                        std::get<0>(mDimensions),std::get<1>(mDimensions));
    return rect;
}


void RectCell::mousePressEvent(QGraphicsSceneMouseEvent *e){

    if(e->button() == Qt::LeftButton){
         recolor(QColor(108,147,209,255));
         sendTool(std::get<0>(mIndices),std::get<1>(mIndices), mToolName);
         mToolName = "edited";
    }
    //Right click to erase the color
    if(e->button() == Qt::RightButton){
        recolor(Qt::white);
        mToolName = "none";
    }
}

void RectCell::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
     recolor(Qt::gray);
}

void RectCell::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    //NOTE:
    // STRING is currently a placeholder for tool types!
    // I will make a tool bar for MapView, and then we will
    // be cooking with gas, but till then, naw.
    if(mToolName == "none"){
        recolor(Qt::white);
    }
}


void RectCell::recolor(QColor color)
{
    mBrush = QBrush(color);
    update();
}


void RectCell::receiveTool()
{
   qDebug() << std::get<0>(mCoords);
}


void RectCell::sendTool(qreal i, qreal j, QString string)
{
 emit emitTool(i,j);
}


