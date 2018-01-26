#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"
#include "tiletemplate.h"

#include <QObject>
#include <QSize>
#include <QSharedPointer>

class TileMap : public QObject
{
    Q_OBJECT

public:
    TileMap(QSize mapSize,
            QObject *parent = nullptr);

    ~TileMap();

    Tile &tileAt(int x, int y);
    const Tile &cTileAt(int x, int y) const;

    void setTile(int x, int y, SharedTileTemplate tileTemplate);

    //sets this tile to the default
    void clearTile(int x, int y) { setTile(x, y, nullptr); }

    //sets the whole map to the default
    void clear();

    QSize mapSize() const { return mMapSize; }

    //changes the size of the map. If the size is reduced, tiles will be lost (resizes around top left corner)
    void resizeMap(QSize newSize);

signals:
    void tileChanged(int x, int y);
    void resized();

private:
    //2D array of Tile*. If mMap[x][y]->isEmpty() then ground is shown
    Tile ***mMap;

    QSize mMapSize;
};

typedef QSharedPointer<TileMap> SharedTileMap;

#endif // TILEMAP_H
