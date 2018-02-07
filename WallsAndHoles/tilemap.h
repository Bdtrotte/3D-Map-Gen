#ifndef TILEMAP_H
#define TILEMAP_H

#include "array2d.h"
#include "tile.h"
#include "tiletemplate.h"
#include "tiletemplateset.h"

#include <QObject>
#include <QSize>
#include <QSharedPointer>

enum Properties{
    NoProperty = 0,
    OutdoorMap = 1,
    IndoorMap  = 2
};

class TileMap : public QObject
{
    Q_OBJECT

public:
    TileMap(QSize mapSize,
            QObject *parent = nullptr);


    Tile &tileAt(int x, int y);
    const Tile &cTileAt(int x, int y) const;

    /**
     * @brief Returns a representation of this map as an Array2D.
     *
     * Useful to access special Array2D iterators.
     *
     * @return An Array2D such that arr(x, y) == tileAt(x, y)
     */
    const Array2D<QSharedPointer<Tile>> &getArray2D() const;

    void setTile(int x, int y, SharedTileTemplate tileTemplate);

    //sets this tile to the default
    void clearTile(int x, int y) { setTile(x, y, nullptr); }

    //sets the whole map to the default
    void clear();

    QSize mapSize() const { return mMap.size(); }

    int width() const { return mMap.size().width(); }
    int height() const { return mMap.size().height(); }

    //changes the size of the map. If the size is reduced, tiles will be lost (resizes around top left corner)
    void resizeMap(QSize newSize);

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }

    //simply add a dependent templateSet for saving/reloading,
    //run updateDepend to check if this dependent it really necessary
    void setDepend(SharedTileTemplateSet templateSet){ mDependencies.push_back(templateSet); }
    //remove all dependencies that are no longer used in this tilemap.
    void updateDepend();

    QVector<SharedTileTemplateSet> dependencies() const { return mDependencies; }

    const Array2D<QSharedPointer<Tile>> &cTiles() const { return mMap; }

    void setProperty(Properties property);
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

    int mProperty;
};

typedef QSharedPointer<TileMap> SharedTileMap;

#endif // TILEMAP_H
