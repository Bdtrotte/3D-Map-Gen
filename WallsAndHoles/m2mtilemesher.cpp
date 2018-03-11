#include "m2mtilemesher.h"

#include "map2mesh.h"
#include "polygon.h"

#include "m2mtilemesher_private.h"
#include "blockypolygontilemesher.h"

M2M::TileNeighborhoodInfo::TileNeighborhoodInfo(const TileMap *tileMap, QPoint centerTilePos)
    : mTileMap(tileMap)
    , mCenterTilePos(centerTilePos)
{
    Q_ASSERT(tileMap != nullptr);
}

const Tile *M2M::TileNeighborhoodInfo::operator ()(int x, int y) const
{
    QPoint pt = QPoint(x, y) + mCenterTilePos;

    if (mTileMap->contains(pt.x(), pt.y()))
        return &mTileMap->cTileAt(pt.x(), pt.y());
    else
        return nullptr;
}

const Tile *M2M::TileNeighborhoodInfo::operator ()(QPoint pt) const
{
    pt += mCenterTilePos;

    if (mTileMap->contains(pt.x(), pt.y()))
        return &mTileMap->cTileAt(pt.x(), pt.y());
    else
        return nullptr;
}

const Tile *M2M::TileNeighborhoodInfo::centerTile() const
{
    if (mTileMap->contains(mCenterTilePos.x(), mCenterTilePos.y()))
        return &mTileMap->cTileAt(mCenterTilePos.x(), mCenterTilePos.y());
    else
        return nullptr;
}

QSharedPointer<M2M::AbstractTileMesher> M2M::TileNeighborhoodInfo::makeMesher() const
{
    return QSharedPointer<M2M::AbstractTileMesher>(new BlockyPolygonTileMesher(*this));
}

QSharedPointer<M2M::AbstractTileMesher> M2M::AbstractTileMesher::getMesherForTile(const TileMap *tileMap, QPoint tilePoint)
{
    return TileNeighborhoodInfo(tileMap, tilePoint).makeMesher();
}

M2M::AbstractTileMesher::AbstractTileMesher(M2M::TileNeighborhoodInfo nbhd)
    : mTileNeighborhood(nbhd) {}
