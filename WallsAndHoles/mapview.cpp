#include "mapview.h"
#include "mapcell.h"

#include <QtMath>
#include <QGraphicsView>
#include <QDebug>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSignalMapper>

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
    , mScale(15)
    , mTileMap(nullptr)
    , mMapCells(0, 0)
    , mMouseHoverRect(new QGraphicsRectItem(0, 0, 1, 1))
    , mPreviewItem(new TileMapPreviewGraphicsItem())
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mMouseHoverRect->setPen(Qt::NoPen);
    mMouseHoverRect->setBrush(QColor(100, 100, 100, 50));
    mMouseHoverRect->setZValue(200);
    mMouseHoverRect->hide();

    setMouseTracking(true);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::gray);
    mPreviewItem->setZValue(100);
    scene->addItem(mPreviewItem);
    scene->addItem(mMouseHoverRect);
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
    float d = event->angleDelta().y();
    if (event->modifiers() & Qt::ControlModifier) mScale += d / 1200;
    else QGraphicsView::wheelEvent(event);//else normal behavior

    QTransform t;
    float s = qPow(M_E, mScale);
    t.scale(s, s);
    setTransform(t);
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

void MapView::setMap(TileMap *tileMap)
{
    clear();

    if (mTileMap)
        disconnect(mTileMap);

    mTileMap = tileMap;

    reMakeMap();

    connect(mTileMap, &TileMap::resized,
            this, &MapView::mapSizeChanged);
}

void MapView::mapSizeChanged()
{
    reMakeMap();
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF curMousePoint = mapToScene(event->pos());
    QPoint curMouseCell(curMousePoint.x(), curMousePoint.y());

    if (curMousePoint.x() < 0) curMouseCell.setX(curMousePoint.x() - 1);
    if (curMousePoint.y() < 0) curMouseCell.setY(curMousePoint.y() - 1);

    if (curMouseCell != mPreMousePoint) {
        if (event->buttons() & Qt::LeftButton) {
            //entered a new cell while holding leftclick
            emit cellActivated(curMouseCell.x(), curMouseCell.y(), event);
        }

        if (curMouseCell.x() >= 0 && curMouseCell.x() < mMapCells.size().width()
                && curMouseCell.y() >= 0 && curMouseCell.y() < mMapCells.size().height()) {
            mMouseHoverRect->setPos(curMouseCell.x(), curMouseCell.y());
            mMouseHoverRect->show();

            // Emit a hovered signal for this cell.
            emit cellHovered(curMouseCell.x(), curMouseCell.y(), event);
        } else if (mPreMousePoint.x() >= 0 && mPreMousePoint.x() < mMapCells.size().width()
                   && mPreMousePoint.y() >= 0 && mPreMousePoint.y() < mMapCells.size().height()) {
            //if the current cell is outside the map, and the previous cell was inside the map

            mMouseHoverRect->hide();

            emit mouseExitedMap(event);
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
        emit cellClicked(mPreMousePoint.x(), mPreMousePoint.y(), event);
        emit cellActivated(mPreMousePoint.x(), mPreMousePoint.y(), event);
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit cellReleased(mPreMousePoint.x(), mPreMousePoint.y(), event);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MapView::reMakeMap()
{
    clear();

    if (!mTileMap) return;

    QSize mapSize = mTileMap->mapSize();
    mMapCells.resize(mapSize.width(), mapSize.height());

    for(int y = 0; y < mTileMap->mapSize().height(); ++y) {
        for(int x = 0; x < mTileMap->mapSize().width(); ++x) {
            mMapCells(x, y) = new MapCell(scene(), x, y, mTileMap->cTileAt(x, y));
        }
    }

    mPreviewItem->setClipRect(QRect(QPoint(0, 0), mTileMap->mapSize()));

    setSceneRect(-MAP_BUFFER,
                 -MAP_BUFFER,
                 mTileMap->width() + MAP_BUFFER * 2,
                 mTileMap->height() + MAP_BUFFER * 2);

    float vertScale = geometry().height() / sceneRect().height();
    float horScale = geometry().width() / sceneRect().width();

    float s = std::min(vertScale, horScale);

    QTransform t;
    t.scale(s, s);
    setTransform(t);

    mScale = qLn(s);
}
