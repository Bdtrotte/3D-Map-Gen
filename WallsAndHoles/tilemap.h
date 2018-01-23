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
            const SharedTileTemplate defaultTileTemplate,
            QObject *parent = nullptr);

    ~TileMap();

    Tile &tileAt(int x, int y);
    const Tile &cTileAt(int x, int y) const;

    void setTile(int x, int y, Tile *tile);

    //sets this tile to the default
    void clearTile(int x, int y);

    //sets the whole map to the default
    void clear();

    QSize mapSize() const { return mMapSize; }

    //changes the size of the map. If the size is reduced, tiles will be lost (resizes around top left corner)
    void resizeMap(QSize newSize);

signals:
    void tileChanged(int x, int y);
    void sizeChanged();

private:
    QSharedPointer<Tile> **mMap;

    QSize mMapSize;

    const SharedTileTemplate mDefaultTileTemplate;
};

#endif // TILEMAP_H
