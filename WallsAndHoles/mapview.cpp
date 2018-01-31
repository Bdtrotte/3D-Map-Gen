#include "mapview.h"

#include "mapcell.h"
#include <QGraphicsView>
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>

MapView::MapView(const QRegion &selectedRegion, QWidget *parent)
    : QGraphicsView(parent),
      mScale(0.5),
      mMapCells(0, 0),
      mSelectedRegion(selectedRegion)
{
    setMouseTracking(true);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    setScene(scene);
    QMatrix mat;
    mat.scale(mScale,mScale);
    setMatrix(mat);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    // TODO: make this tied to
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
    QSize size = mMapCells.size();
    for (int x = 0; x < size.width(); ++x)
        for (int y = 0; y < size.width(); ++y)
            delete mMapCells(x, y);

    mMapCells.resize(0, 0);
}

void MapView::createMap(TileMap *tileMap)
{
    clear();

    QSize mapSize = tileMap->mapSize();

    mMapCells.resize(mapSize.width(), mapSize.height());

    for(int y = 0; y < tileMap->mapSize().height(); ++y) {
        for(int x = 0; x < tileMap->mapSize().width(); ++x) {
            mMapCells(x, y) = new MapCell(scene(), x, y, tileMap->cTileAt(x, y));
            if (mSelectedRegion.contains(QPoint(x, y)))
                mMapCells(x, y)->setHighlightBrush(QColor(200, 200, 255, 80)); //TODO: This should be defined somewhere meaningful
        }
    }
}

void MapView::genHeightMap(TileMap *tileMap){
    for(int x = 0; x < tileMap->mapSize().width();++x){
        for(int y = 0; y < tileMap->mapSize().height(); ++y){
            float height = tileMap->tileAt(x,y).relativeHeight();
            if(height < 0){
                //if height is less than 0 the heightMap will be red
                float sig = (.25*height)/(.25*(height - 1));
                int colorVal = 255-(255*sig);
                int alpha = (150*sig);
                mMapCells(x,y)->changeHeightMap(x, y, QColor(255, colorVal, colorVal, alpha));
            }
            else{
                //if height is greater than 0 the heightMap will be green
                float sig = (.25*height)/(.25*(height + 1));
                int colorVal = 255-(255*sig);
                int alpha = (150*sig);
                mMapCells(x,y)->changeHeightMap(x, y, QColor(colorVal, 255, colorVal, alpha));
            }
        }
    }
}

void MapView::clearHeightMap(TileMap *tileMap){
    for(int x = 0; x < tileMap->mapSize().width();++x)
        for(int y = 0; y < tileMap->mapSize().height(); ++y)
            mMapCells(x,y)->changeHeightMap(x, y, Qt::transparent);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF curMousePoint = mapToScene(event->pos()) / 30;
    QPoint curMouseCell(curMousePoint.x(), curMousePoint.y());

    if (curMousePoint.x() < 0) curMouseCell.setX(-1);
    if (curMousePoint.y() < 0) curMouseCell.setY(-1);

    if (curMouseCell != mPreMousePoint) {
        if (mPreMousePoint.x() >= 0 && mPreMousePoint.x() < mMapCells.size().width()
                && mPreMousePoint.y() >= 0 && mPreMousePoint.y() < mMapCells.size().height()) {
            if (mSelectedRegion.contains(mPreMousePoint))
                mMapCells(mPreMousePoint.x(), mPreMousePoint.y())->setHighlightBrush(QColor(200, 200, 255, 80)); //TODO: This should be defined somewhere meaningful
            else
                mMapCells(mPreMousePoint.x(), mPreMousePoint.y())->setHighlightBrush(Qt::NoBrush);
        }

        if (curMouseCell.x() >= 0 && curMouseCell.x() < mMapCells.size().width()
                && curMouseCell.y() >= 0 && curMouseCell.y() < mMapCells.size().height()) {
            //Just manually setting highlight color here, but TODO: make this a configurable variable somewhere else
            mMapCells(curMouseCell.x(), curMouseCell.y())->setHighlightBrush(QColor(0, 0, 0, 20));

            if (event->buttons() & Qt::LeftButton) {
                //entered a new cell while holding leftclick
                emit cellActivated(curMouseCell.x(), curMouseCell.y());
            }
        }

        mPreMousePoint = curMouseCell;
    }

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

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (mPreMousePoint.x() >= 0 && mPreMousePoint.x() < mMapCells.size().width()
                && mPreMousePoint.y() >= 0 && mPreMousePoint.y() < mMapCells.size().height()) {
            emit cellClicked(mPreMousePoint.x(), mPreMousePoint.y());
            emit cellActivated(mPreMousePoint.x(), mPreMousePoint.y());
        }
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (mPreMousePoint.x() >= 0 && mPreMousePoint.x() < mMapCells.size().width()
                && mPreMousePoint.y() >= 0 && mPreMousePoint.y() < mMapCells.size().height())
            emit cellReleased(mPreMousePoint.x(), mPreMousePoint.y());
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}
