#include "mapview.h"
#include "mapcell.h"

#include <QGraphicsView>
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSignalMapper>

MapView::MapView(const QRegion &selectedRegion, QWidget *parent)
    : QGraphicsView(parent)
    , mScale(0.5)
    , mMapCells(0, 0)
    , mPreviewItem(new TileMapPreviewGraphicsItem())
    , mSelectedRegion(selectedRegion)
    , mToolBar(new QToolBar(this))
{
    setupViewToolBar();
    setMouseTracking(true);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    mPreviewItem->setZValue(100);
    scene->addItem(mPreviewItem);
    setScene(scene);

    QMatrix mat;
    mat.scale(mScale,mScale);
    setMatrix(mat);
}

MapView::~MapView()
{
    delete mPreviewItem;
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
    for (int x = 0; x < size.width(); ++x){
        for (int y = 0; y < size.height(); ++y){
            delete mMapCells(x, y);
        }
    }
    mMapCells.resize(0, 0);
}

void MapView::createMap(TileMap *tileMap)
{
    clear();

    if (!tileMap) return;

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

            // Emit a hovered signal for this cell.
            emit cellHovered(curMouseCell.x(), curMouseCell.y());

            if (event->buttons() & Qt::LeftButton) {
                //entered a new cell while holding leftclick
                emit cellActivated(curMouseCell.x(), curMouseCell.y());
            }
        } else {
            emit mouseExitedMap();
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

void MapView::setupViewToolBar()
{
    mNoView = new QAction("No View", this);
    mDefaultView = new QAction("Default View", this);
    mHeightView = new QAction("Height Map", this);

    mNoView->setCheckable(true);
    mDefaultView->setCheckable(true);
    mHeightView->setCheckable(true);

    mDefaultView->setChecked(true);

    connect(mNoView, &QAction::toggled, this, &MapView::setNoView);
    connect(mDefaultView, &QAction::toggled, this, &MapView::setDefaultView);
    connect(mHeightView,  &QAction::toggled, this, &MapView::setHeightMap);

    mToolBar->addAction(mNoView);
    mToolBar->addAction(mDefaultView);
    mToolBar->addAction(mHeightView);
    mToolBar->setAutoFillBackground(true);
    mToolBar->show();
}

void MapView::setNoView(bool state)
{
    if (state) {
        if (mDefaultView->isChecked())
            mDefaultView->setChecked(false);
        if (mHeightView->isChecked())
            mHeightView->setChecked(false);
    } else {
        if (!mHeightView->isChecked() && !mDefaultView->isChecked())
            mDefaultView->setChecked(true);
    }
}

void MapView::setDefaultView(bool state)
{
    if (state) {
        mNoView->setChecked(false);
    } else {
        if (!mHeightView->isChecked())
            mNoView->setChecked(true);
    }

    for(int x = 0; x<mMapCells.size().width(); ++x)
        for(int y = 0; y<mMapCells.size().height(); ++y)
            mMapCells(x,y)->setGraphics(DefaultView, state);
}

void MapView::setHeightMap(bool state)
{
    if (state) {
        mNoView->setChecked(false);
    } else {
        if (!mDefaultView->isChecked())
            mNoView->setChecked(true);
    }

    for(int x = 0; x<mMapCells.size().width(); ++x)
        for(int y = 0; y<mMapCells.size().height(); ++y)
            mMapCells(x,y)->setGraphics(HeightMapView, state);
}
