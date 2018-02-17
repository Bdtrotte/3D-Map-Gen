#ifndef TILEMAP_H
#define TILEMAP_H

#include "array2d.h"
#include "tile.h"
#include "tiletemplate.h"
#include "tiletemplateset.h"

#include <QObject>
#include <QSize>
#include <QSharedPointer>
#include <QMutex>

class TileMap : public QObject
{
    Q_OBJECT

public:
    TileMap(QSize mapSize,
            bool isIndoors,
            bool hasCeiling,
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

    void setTile(int x, int y, TileTemplate *tileTemplate);

    //sets this tile to the default
    void clearTile(int x, int y) { setTile(x, y, nullptr); }

    //sets the whole map to the default
    void clear();

    QSize mapSize() const { return mMap.size(); }

    int width() const { return mMap.size().width(); }
    int height() const { return mMap.size().height(); }
    bool isIndoor() const { return mIsIndoors; }
    bool hasCeiling() const { return mHasCeiling; }

    //changes the size of the map. If the size is reduced, tiles will be lost (resizes around top left corner)
    void resizeMap(QSize newSize);

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }
    const Array2D<QSharedPointer<Tile>> &cTiles() const { return mMap; }

    /**
     * @brief tileTemplateUsed
     * Returns true if some tile in the map uses this tileTemplate
     * @param tileTemplate
     * The tile template to check.
     */
    bool isTileTemplateUsed(TileTemplate *tileTemplate);

    /**
     * @brief tileTemplateSetUsed
     * Returns true if some tile in the map uses any of the tileTemplates
     * of the given set
     * @param tileTemplateSet
     * The set to check
     */
    bool isTileTemplateSetUsed(TileTemplateSet *tileTemplateSet);

    /**
     * @brief removingTileTemplateSet
     * Called when a tileTemplateSet is going to be removed.
     * Any tiles using templates from the set will be cleared.
     * @param tileTemplateSet
     */
    void removingTileTemplateSet(TileTemplateSet *tileTemplateSet);

    TileTemplateSet *defaultTileTemplateSet() { return mDefaultTileTemplateSet; }

public slots:
    void tilePinged(int x, int y);

signals:
    void tileChanged(int x, int y);
    void resized();

    /**
     * @brief Sent out whenever the map is changed in any way. Happens after tileChanged() and resized() signals.
     */
    void mapChanged();

private:
    /**
     * @brief The TilePingReceiveMode enum
     * How a tilePinged singnal should be received.
     */
    enum TilePingReceiveMode {
        None,     //No action taken on ping reception.
        SetCheck, //Set mTilePinged to true on ping reception.
        Collect   //Add the pinded tile to mPingedTiles.
    };

    //2D array of Tile*. If mMap[x][y]->isEmpty() then ground is shown
    Array2D<QSharedPointer<Tile>> mMap;

    //General Properties of the map:
    bool mIsIndoors;
    bool mHasCeiling;

    //default save path of this tilemap object, can be changed when using "save as" command.
    QString mSavePath;

    TilePingReceiveMode mTilePingReceiveMode;
    //set to true when mTilePingReceiveMode == SetCheck, and a tilePinged is received.
    //Should be carefully be set to false elsewhere
    bool mTilePinged;
    QVector<QSharedPointer<Tile>> mPingedTiles;

    TileTemplateSet *mDefaultTileTemplateSet;

    QMutex mPingingMutex;
};

#endif // TILEMAP_H
