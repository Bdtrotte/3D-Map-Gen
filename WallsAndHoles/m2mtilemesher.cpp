#include "m2mtilemesher.h"

#include "map2mesh.h"
#include "polygon.h"

#include "m2mtilemesher_private.h"
#include "blockypolygontilemesher.h"


QSharedPointer<M2M::AbstractTileMesher> M2M::AbstractTileMesher::getMesherForTile(
        Array2D<const Tile *> neighborhood,
        const AbstractTileMesher *oldMesher)
{
    TileNeighborhoodInfo newNeighborhood(neighborhood);


    // If the neighborhood didn't change, don't supply a new mesher.
    if (oldMesher != nullptr && newNeighborhood == oldMesher->mTileNeighborhood)
        return nullptr;

    // If the neighborhood changed, supply a new mesher that can properly mesh it.
    else
        return newNeighborhood.makeMesher();
}


M2M::AbstractTileMesher::AbstractTileMesher(TileNeighborhoodInfo nbhd)
    : mTileNeighborhood(nbhd) {}

/* BEGIN TileInfo */

M2M::TileInfo::TileInfo()
    : mIsNull(true) {}

M2M::TileInfo::TileInfo(const Tile &tile)
    : mIsNull(false)
    , mTopHeight(tile.height())
    , mThickness(tile.thickness())
    , mCenter(tile.position())
{
    // TODO
    //   Get ground properties from somewhere.


    mGround.groundHeight = 0;
    mGround.groundMaterial.ambient = 1;
    mGround.groundMaterial.diffuse = 1;
    mGround.groundMaterial.specular = 1;
    mGround.groundMaterial.shininess = 1;
    mGround.groundImage = ImageInfo(getDefaultImage());


    if (tile.hasTileTemplate()) {
        mIsGround = false;

        const auto &topMat = *tile.tileTemplate()->topMaterial();

        if (!topMat.texture().isNull())
            mTopImage = ImageInfo(topMat.texture());
        else
            mTopImage = ImageInfo(getDefaultImage());

        mTopMaterial.ambient = topMat.ambient();
        mTopMaterial.diffuse = topMat.diffuse();
        mTopMaterial.specular = topMat.specular();
        mTopMaterial.shininess = topMat.shininess();

        if (tile.tileTemplate()->hasSideMaterial()) {
            const auto &sideMat = *tile.tileTemplate()->sideMaterial();

            if (!topMat.texture().isNull())
                mSideImage = ImageInfo(sideMat.texture());
            else
                mSideImage = ImageInfo(getDefaultImage());

            mSideMaterial.ambient = sideMat.ambient();
            mSideMaterial.diffuse = sideMat.diffuse();
            mSideMaterial.specular = sideMat.specular();
            mSideMaterial.shininess = topMat.shininess();
        } else {
            if (!topMat.texture().isNull())
                mSideImage = ImageInfo(topMat.texture());
            else
                mSideImage = ImageInfo(getDefaultImage());

            mSideMaterial.ambient = topMat.ambient();
            mSideMaterial.diffuse = topMat.diffuse();
            mSideMaterial.specular = topMat.specular();
            mSideMaterial.shininess = topMat.shininess();
        }
    } else {
        mIsGround = true;

        mTopImage = mSideImage = ImageInfo(getDefaultImage());

        mTopMaterial.ambient = mSideMaterial.ambient
                = mTopMaterial.diffuse = mSideMaterial.diffuse
                = mTopMaterial.specular = mSideMaterial.specular
                = mTopMaterial.shininess = mSideMaterial.shininess = 1;
    }
}


bool M2M::TileInfo::operator ==(const TileInfo &other) const
{
    if (isNull() && other.isNull())
        return true;

    return topHeight() == other.topHeight()
            && thickness() == other.thickness()
            && center() == other.center()
            && groundInfo() == other.groundInfo()
            && topImage() == other.topImage()
            && sideImage() == other.sideImage()
            && topMaterial() == other.topMaterial()
            && sideMaterial() == other.sideMaterial();
}

bool M2M::TileInfo::operator !=(const TileInfo &other) const
{
    return !(*this == other);
}


/* END TileInfo */

/* BEGIN TileNeighborhoodInfo */

M2M::TileNeighborhoodInfo::TileNeighborhoodInfo(Array2D<const Tile *> nbhd)
    : mTileInfos(3, 3)
{
    for (QPoint idx : nbhd.indices()) {
        const Tile *neighboringTile = nbhd(idx);

        if (neighboringTile == nullptr)
            mTileInfos(idx) = TileInfo();
        else
            mTileInfos(idx) = TileInfo(*neighboringTile);
    }
}

bool M2M::TileNeighborhoodInfo::operator ==(const TileNeighborhoodInfo &other) const
{
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (mTileInfos(x, y) != other(x, y))
                return false;
        }
    }

    return true;
}

bool M2M::TileNeighborhoodInfo::operator !=(const TileNeighborhoodInfo &other) const
{
    return !(*this == other);
}

const M2M::TileInfo &M2M::TileNeighborhoodInfo::operator ()(int x, int y) const
{
    return mTileInfos(x, y);
}

const M2M::TileInfo &M2M::TileNeighborhoodInfo::operator ()(QPoint pt) const
{
    return (*this)(pt.x(), pt.y());
}

const M2M::TileInfo &M2M::TileNeighborhoodInfo::centerTile() const
{
    return mTileInfos(1, 1);
}

QSharedPointer<M2M::AbstractTileMesher> M2M::TileNeighborhoodInfo::makeMesher() const
{
    // TODO: Analyze neighborhood and return appropriate mesher.
    // TODO: For now, return an instance of the blocky mesher.

    return QSharedPointer<M2M::AbstractTileMesher>(new BlockyPolygonTileMesher(*this));
}

/* END TileNeighborhoodInfo */




SharedImageAndSource M2M::TileInfo::DefaultImage = nullptr;
SharedImageAndSource M2M::TileInfo::getDefaultImage()
{
    if (DefaultImage.isNull())
        DefaultImage = ImageAndSource::getSharedImageAndSource("://images/textures/grassTexture.jpg");

    return DefaultImage;
}
