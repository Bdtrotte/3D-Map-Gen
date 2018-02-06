#include "mapcell.h"
#include <QDebug>

MapCell::MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent)
    : QObject(parent)
    , mScene(scene)
    , mX(x)
    , mY(y)
    , mTile(tile)
    , mViewFlag(1)
{
    mGraphics = new MapCellGraphicsItem(x*30+10, y*30+10, 10, 10, mTile);
    mGraphics->setPen(Qt::NoPen);
    if (mTile.hasTileTemplate()){
        mGraphics->setBrush(mTile.tileTemplate()->color());
    }
    else
        mGraphics->setBrush(Qt::NoBrush);

    mHighlight = new QGraphicsRectItem(x*30, y*30, 30, 30);
    mHighlight->setZValue(1);
    mHighlight->setBrush(Qt::NoBrush);
    mHighlight->setPen(Qt::NoPen);

    mBackground = new QGraphicsRectItem(x*30, y*30, 30, 30);
    mBackground->setZValue(-1);
    mBackground->setBrush(Qt::white);
    mBackground->setPen(QPen(Qt::black, 0, Qt::DashLine));

    mScene->addItem(mGraphics);
    mScene->addItem(mHighlight);
    mScene->addItem(mBackground);

    connect(&mTile, &Tile::tileChanged,
            this, &MapCell::tileChanged);
}

MapCell::~MapCell()
{
    mScene->removeItem(mGraphics);
    mScene->removeItem(mHighlight);
    mScene->removeItem(mBackground);

    delete mGraphics;
    delete mHighlight;
    delete mBackground;
}

void MapCell::tileChanged()
{
    if (mTile.hasTileTemplate())
        mGraphics->setBrush(mTile.tileTemplate()->color());
    else
        mGraphics->setBrush(Qt::NoBrush);
    setGraphics(mViewFlag);
}

void MapCell::setGraphics(int viewFlag){
    mViewFlag = viewFlag;
    mGraphics->setView(mViewFlag);
    mScene->update();
}

