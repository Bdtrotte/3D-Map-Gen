#include "mapcell.h"
#include <QDebug>

MapCell::MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent)
    : QObject(parent)
    , mScene(scene)
{
    mGraphics = new MapCellGraphicsItem(x*30, y*30, 30, 30, tile);

    mGrid = new QGraphicsRectItem(x*30, y*30, 30, 30);
    mGrid->setZValue(2);
    mGrid->setBrush(Qt::NoBrush);
    mGrid->setPen(QPen(Qt::black, 0, Qt::DashLine));

    mBackground = new QGraphicsRectItem(x*30, y*30, 30, 30);
    mBackground->setZValue(-1);
    mBackground->setBrush(Qt::white);
    mBackground->setPen(Qt::NoPen);

    mScene->addItem(mGraphics);
    mScene->addItem(mGrid);
    mScene->addItem(mBackground);

    connect(&tile, &Tile::tileChanged,
            this, &MapCell::tileChanged);
}

MapCell::~MapCell()
{
    mScene->removeItem(mGraphics);
    mScene->removeItem(mGrid);
    mScene->removeItem(mBackground);

    delete mGraphics;
    delete mGrid;
    delete mBackground;
}

void MapCell::tileChanged()
{
    mGraphics->update();
}

void MapCell::setGraphics(MapViewMode viewMode, bool enabled)
{
    mGraphics->setViewMode(viewMode, enabled);
    mGraphics->update();
}

