#include "mapcell.h"
#include "mapcellgraphicsitem.h"
#include <QDebug>

MapCell::MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent)
    : QObject(parent)
    , mScene(scene)
    , mX(x)
    , mY(y)
    , mTile(tile)
    , mViewFlag(defaultView)
{
    mGraphics = new QGraphicsRectItem(x * 30 + 10, y * 30 + 10, 10, 10);
    mGraphics->setPen(Qt::NoPen);
    if (mTile.hasTileTemplate())
        mGraphics->setBrush(mTile.tileTemplate()->color());
    else
        mGraphics->setBrush(Qt::NoBrush);

    mHighlight = new QGraphicsRectItem(x * 30, y * 30, 30, 30);
    mHighlight->setZValue(1);
    mHighlight->setBrush(Qt::NoBrush);
    mHighlight->setPen(Qt::NoPen);

    mBackground = new QGraphicsRectItem(x * 30, y * 30, 30, 30);
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

void MapCell::setGraphics(int flag){
    mViewFlag = flag;

    switch (mViewFlag) {
    case defaultView:
        mGraphics->setRect(mX*30+10, mY*30+10, 10, 10);
        if (mTile.hasTileTemplate())
            mGraphics->setBrush(mTile.tileTemplate()->color());
        else
            mGraphics->setBrush(Qt::NoBrush);
        break;
    case heightMapView:
    {
        float height = mTile.height();
        if(height < 0){
            //if the height is less than 0 the heightMap will be red
            float sig = (.25*height)/(.25*(height - 1));
            int colorVal = 255-(255*sig);
            int alpha = 150*sig;
            mGraphics->setBrush(QBrush(QColor(255, colorVal, colorVal, alpha)));
            mGraphics->setRect(mX*30, mY*30, 30, 30);
        }
        else{
            //if height is greater than 0 the heightMap will be green
            float sig = (.25*height)/(.25*(height + 1));
            int colorVal = 255-(255*sig);
            int alpha = 150*sig;
            mGraphics->setBrush(QBrush(QColor(colorVal, 255, colorVal, alpha)));
            mGraphics->setRect(mX*30, mY*30, 30, 30);
        }
        break;
    }
    default:
        break;
    }
}

