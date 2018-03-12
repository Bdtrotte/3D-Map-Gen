#ifndef M2MTILEMESHER_H
#define M2MTILEMESHER_H

#include <QVector2D>
#include <QSharedPointer>

#include "tilemap.h"
#include "simpletexturedobject.h"

namespace M2M {

class AbstractTileMesher;

class TileNeighborhoodInfo
{
public:
    TileNeighborhoodInfo(const TileMap *tileMap, QPoint centerTilePos);

    const Tile *operator ()(int x, int y) const;
    const Tile *operator ()(QPoint pt) const;

    const Tile *centerTile() const;

    QSharedPointer<AbstractTileMesher> makeMesher() const;

private:
    const TileMap *mTileMap;

    QPoint mCenterTilePos;
};


/**
 * @brief A collection of methods for generating tile mesh data.
 */
class AbstractTileMesher
{
public:

    virtual ~AbstractTileMesher() {}

    /**
     * @brief Returns a TileMesher instance that will create the mesh for the given tile,
     * or, if the tile has not changed (determined by oldMesher), returns nullptr.
     */
    static QSharedPointer<AbstractTileMesher> getMesherForTile(const TileMap *tileMap, QPoint tilePoint);


    /**
     * @brief makeMesh  Creates the mesh for this tile.
     * @return          A list of objects that contain the mesh info for the tile.
     */
    virtual QVector<QSharedPointer<SimpleTexturedObject>> makeMesh(QVector2D offset) = 0;


protected:
    AbstractTileMesher(TileNeighborhoodInfo nbhd);

    /**
     * @brief Information for each tile in the 3x3 neighborhood of the tile to which this mesher is associated.
     */
    TileNeighborhoodInfo mTileNeighborhood;
};

}

#endif // M2MTILEMESHER_H
