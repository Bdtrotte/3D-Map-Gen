#ifndef TILEMAP_H
#define TILEMAP_H

#include "array2d.h"
#include "tile.h"
#include "tiletemplate.h"
#include "tiletemplateset.h"

#include <QObject>
#include <QSize>
#include <QSharedPointer>

class TileMap : public QObject
{
    Q_OBJECT

public:
    TileMap(QSize mapSize,
            QObject *parent = nullptr);

    Tile &tileAt(int x, int y);
    const Tile &cTileAt(int x, int y) const;

    void setTile(int x, int y, SharedTileTemplate tileTemplate);

    //sets this tile to the default
    void clearTile(int x, int y) { setTile(x, y, nullptr); }

    //sets the whole map to the default
    void clear();

    QSize mapSize() const { return mMap.size(); }

    //changes the size of the map. If the size is reduced, tiles will be lost (resizes around top left corner)
    void resizeMap(QSize newSize);

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }

    //If tile-template link present, update tile id accordingly, otherwise create link according to tile id.
    void setDepend(SharedTileTemplateSet templateSet){ mDependencies.push_back(templateSet); }
    //remove all dependencies that are no longer used in this tilemap.
    void updateDepend();

    QVector<SharedTileTemplateSet> dependencies() const { return mDependencies; }

    Array2D<QSharedPointer<Tile>> &tiles() { return mMap; }
    const Array2D<QSharedPointer<Tile>> &cTiles() const { return mMap; }


signals:
    void tileChanged(int x, int y);
    void resized();

private:
    //2D array of Tile*. If mMap[x][y]->isEmpty() then ground is shown
    Array2D<QSharedPointer<Tile>> mMap;

    //holding a reference to all tileTemplateSet that this tilemap depends on
    QVector<SharedTileTemplateSet> mDependencies;
    //default save path of this tilemap object, can be changed when using "save as" command.
    QString mSavePath;
};

typedef QSharedPointer<TileMap> SharedTileMap;

#endif // TILEMAP_H
