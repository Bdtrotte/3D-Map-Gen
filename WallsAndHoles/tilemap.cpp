#include "tilemap.h"

#include <QDebug>

TileMap::TileMap(QSize mapSize,
                 QObject *parent)
    : QObject(parent)
    , mMap(mapSize.width(), mapSize.height())
{
    for (int x = 0; x < mMap.size().width(); ++x) {
        for (int y = 0; y < mMap.size().height(); ++y) {
            mMap(x, y) = QSharedPointer<Tile>::create(nullptr, x, y, this);
            connect(mMap(x, y).data(), &Tile::tileChanged,
                    this, &TileMap::tileChanged);
        }
    }
}


Tile &TileMap::tileAt(int x, int y)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMap.size().width());
    Q_ASSERT(y < mMap.size().height());

    return *mMap(x, y);
}

const Tile &TileMap::cTileAt(int x, int y) const
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMap.size().width());
    Q_ASSERT(y < mMap.size().height());

    return *mMap(x, y);
}

const Array2D<QSharedPointer<Tile>> &TileMap::getArray2D() const
{
    return mMap;
}

void TileMap::setTile(int x, int y, SharedTileTemplate tileTemplate)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMap.size().width());
    Q_ASSERT(y < mMap.size().height());

    mMap(x, y)->resetTile(tileTemplate);
}

void TileMap::clear()
{
    for (int x = 0; x < mMap.size().width(); ++x)
        for (int y = 0; y < mMap.size().height(); ++y)
            clearTile(x, y);
}

void TileMap::resizeMap(QSize newSize)
{
    Q_ASSERT(newSize.width() >= 1);
    Q_ASSERT(newSize.height() >= 1);

    QSize oldSize = mMap.size();
    mMap.resize(newSize.width(), newSize.height());

    int maxX = newSize.width();
    if (oldSize.width() < newSize.width()) {
        maxX = oldSize.width();
        for (int x = oldSize.width(); x < newSize.width(); ++x) {
            for (int y = 0; y < newSize.height(); ++y) {
                mMap(x, y) = QSharedPointer<Tile>::create(nullptr, x, y, this);
                connect(mMap(x, y).data(), &Tile::tileChanged,
                        this, &TileMap::tileChanged);
            }
        }
    }

    if (oldSize.height() < newSize.height()) {
        for (int x = 0; x < maxX; ++x) {
            for (int y = oldSize.height(); y < newSize.height(); ++y) {
                mMap(x, y) = QSharedPointer<Tile>::create(nullptr, x, y, this);
                connect(mMap(x, y).data(), &Tile::tileChanged,
                        this, &TileMap::tileChanged);
            }
        }
    }

    emit resized();
}

void TileMap::setDepend(SharedTileTemplateSet templateSet){
    if(mDependencies.indexOf(templateSet)==-1){
        mDependencies.push_back(templateSet);
    }
}

void TileMap::updateDepend(){
    QVector<bool> valid(mDependencies.size());
    for (int x = 0; x < mMap.size().width(); ++x) {
        for (int y = 0; y < mMap.size().height(); ++y) {
            for(int i=0; i<mDependencies.size(); i++){
                if(mDependencies[i]->cTileTemplates().indexOf(mMap(x, y)->tileTemplate())>=0){
                    valid[i]=true;
                }
            }
        }
    }
    QVector<SharedTileTemplateSet> newDependencies;
    for(int i=0; i<valid.size(); i++){
        if(valid[i])
            newDependencies.push_back(mDependencies[i]);
    }
    mDependencies = newDependencies;
}
