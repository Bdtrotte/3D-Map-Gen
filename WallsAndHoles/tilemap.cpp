#include "tilemap.h"

TileMap::TileMap(QSize mapSize,
                 QObject *parent)
    : QObject(parent)
    , mMapSize(mapSize)
{
    mMap = (Tile ***)malloc(sizeof(Tile **) * mMapSize.width());
    for (int x = 0; x < mMapSize.width(); ++x) {
        mMap[x] = (Tile **)malloc(sizeof(Tile *) * mMapSize.height());

        for (int y = 0; y < mMapSize.height(); ++y) {
            mMap[x][y] = new Tile(nullptr, x, y);
            connect(mMap[x][y], &Tile::tileChanged,
                    this, &TileMap::tileChanged);
        }
    }
}

TileMap::~TileMap()
{
    for (int x = 0; x < mMapSize.width(); ++x)
        free(mMap[x]);
    free(mMap);
}

Tile &TileMap::tileAt(int x, int y)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMapSize.width());
    Q_ASSERT(y < mMapSize.height());

    return *mMap[x][y];
}

const Tile &TileMap::cTileAt(int x, int y) const
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMapSize.width());
    Q_ASSERT(y < mMapSize.height());

    return *mMap[x][y];
}

void TileMap::setTile(int x, int y, SharedTileTemplate tileTemplate)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMapSize.width());
    Q_ASSERT(y < mMapSize.height());

    mMap[x][y]->resetTile(tileTemplate);
}

void TileMap::clear()
{
    for (int x = 0; x < mMapSize.width(); ++x)
        for (int y = 0; y < mMapSize.height(); ++y)
            clearTile(x, y);
}

void TileMap::resizeMap(QSize newSize)
{
    Q_ASSERT(newSize.width() >= 1);
    Q_ASSERT(newSize.height() >= 1);

    if (newSize.height() != mMapSize.height()) {
        for (int x = 0; x < mMapSize.width(); ++x) {
            if (newSize.height() < mMapSize.height())
                for (int y = newSize.height(); y < mMapSize.height(); ++y)
                    delete mMap[x][y];

            mMap[x] = (Tile **)realloc(mMap[x], sizeof(Tile *) * newSize.height());

            if (newSize.height() > mMapSize.height()) {
                for (int y = mMapSize.height(); y < newSize.height(); ++y) {
                    mMap[x][y] = new Tile(nullptr, x, y);
                    connect(mMap[x][y], &Tile::tileChanged,
                            this, &TileMap::tileChanged);
                }
            }
        }
    }

    if (newSize.width() != mMapSize.width()) {
        if (newSize.width() < mMapSize.width()) {
            for (int x = newSize.width(); x < mMapSize.width(); ++x) {
                for (int y = 0; y < newSize.height(); ++y)
                    delete mMap[x][y];
                free(mMap[x]);
            }
        }

        mMap = (Tile ***)realloc(mMap, sizeof(Tile **) * newSize.width());

        if (newSize.width() > mMapSize.width()) {
            for (int x = mMapSize.width(); x < newSize.width(); ++x) {
                mMap[x] = (Tile **)malloc(sizeof(Tile *) * newSize.height());
                for (int y = 0; y < mMapSize.width(); ++y) {
                    mMap[x][y] = new Tile(nullptr, x, y);
                    connect(mMap[x][y], &Tile::tileChanged,
                            this, &TileMap::tileChanged);
                }
            }
        }
    }

    emit resized();
}
