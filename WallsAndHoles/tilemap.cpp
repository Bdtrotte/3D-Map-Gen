#include "tilemap.h"

TileMap::TileMap(QSize mapSize,
                 const SharedTileTemplate defaultTileTemplate,
                 QObject *parent)
    : QObject(parent)
    , mMapSize(mapSize)
    , mDefaultTileTemplate(defaultTileTemplate)
{
    mMap = (QSharedPointer<Tile> **)malloc(sizeof(QSharedPointer<Tile> *) * mapSize.width());
    for (int x = 0; x < mapSize.width(); ++x) {
        mMap[x] = (QSharedPointer<Tile> *)malloc(sizeof(QSharedPointer<Tile>) * mapSize.height());

        for (int y = 0; y < mapSize.height(); ++y) {
            mMap[x][y] = QSharedPointer<Tile>(new Tile(mDefaultTileTemplate, x, y));
            connect(mMap[x][y].data(), &Tile::tileChanged,
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

void TileMap::setTile(int x, int y, QSharedPointer<Tile> tile)
{
    Q_ASSERT(x >= 0);
    Q_ASSERT(y >= 0);
    Q_ASSERT(x < mMapSize.width());
    Q_ASSERT(y < mMapSize.height());

    mMap[x][y] = tile;

    connect(mMap[x][y].data(), &Tile::tileChanged,
            this, &TileMap::tileChanged);

    emit tileChanged(x, y);
}

void TileMap::clearTile(int x, int y)
{
    setTile(x, y, QSharedPointer<Tile>(new Tile(mDefaultTileTemplate, x, y)));
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
            mMap[x] = (QSharedPointer<Tile> *)realloc(mMap[x], sizeof(QSharedPointer<Tile>) * newSize.height());

            if (newSize.height() > mMapSize.height()) {
                for (int y = mMapSize.height(); y < newSize.height(); ++y) {
                    mMap[x][y] = QSharedPointer<Tile>(new Tile(mDefaultTileTemplate, x, y));
                    connect(mMap[x][y].data(), &Tile::tileChanged,
                            this, &TileMap::tileChanged);
                }
            }
        }
    }

    if (newSize.width() != mMapSize.width()) {
        mMap = (QSharedPointer<Tile> **)realloc(mMap, sizeof(QSharedPointer<Tile> *) * newSize.width());

        if (newSize.width() > mMapSize.width()) {
            for (int x = mMapSize.width(); x < newSize.width(); ++x) {
                mMap[x] = (QSharedPointer<Tile> *)malloc(sizeof(QSharedPointer<Tile>) * newSize.height());
                for (int y = 0; y < mMapSize.width(); ++y) {
                    mMap[x][y] = QSharedPointer<Tile>(new Tile(mDefaultTileTemplate, x, y));
                    connect(mMap[x][y].data(), &Tile::tileChanged,
                            this, &TileMap::tileChanged);
                }
            }
        }
    }

    emit sizeChanged();
}
