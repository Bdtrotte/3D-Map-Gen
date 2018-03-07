#include "m2mtilemesher.h"
#include "map2mesh.h"

#include "m2mtilemesher_private.h"


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




M2M::TileBlockyMesher::TileBlockyMesher(TileNeighborhoodInfo nbhd)
    : M2M::AbstractTileMesher(nbhd) {}


QVector<QSharedPointer<SimpleTexturedObject>> M2M::TileBlockyMesher::makeMesh(QVector2D offset)
{
    PartialMeshData mesh;

    makeTopMesh(mesh, offset);

    makeVerticalSideMesh(mesh, M2M_Private::SideDirection::NORTH, offset);
    makeVerticalSideMesh(mesh, M2M_Private::SideDirection::EAST, offset);
    makeVerticalSideMesh(mesh, M2M_Private::SideDirection::SOUTH, offset);
    makeVerticalSideMesh(mesh, M2M_Private::SideDirection::WEST, offset);

    return mesh.constructObjects();
}





void M2M::AbstractTileMesher::makeTopMesh(PartialMeshData &meshData, QVector2D offset)
{
    /*
     * This method just creates a single textured quad for the mesher's center tile.
     * */

    QVector2D t1(1, 1);
    QVector2D t2(0, 1);
    QVector2D t3(0, 0);
    QVector2D t4(1, 0);

    QVector2D offsetFromCenter = mTileNeighborhood.centerTile().offsetFromCenter();
    float thickness = mTileNeighborhood.centerTile().thickness();
    float topHeight = mTileNeighborhood.centerTile().topHeight();

    QVector3D center(offset.x() - offsetFromCenter.x(), topHeight, offset.y() + offsetFromCenter.y());
    QVector3D x(thickness*0.5, 0, 0);
    QVector3D z(0, 0, thickness*0.5);

    meshData.addQuad(Quad(QVector3D(0, 1, 0),
                          mTileNeighborhood.centerTile().topImage(),
                          mTileNeighborhood.centerTile().topMaterial(),
                          center - x + z, t1,
                          center + x + z, t2,
                          center + x - z, t3,
                          center - x - z, t4));
}


void M2M::AbstractTileMesher::makeVerticalSideMesh(PartialMeshData &meshData, M2M_Private::SideDirection sideDirection, QVector2D offset)
{
    using namespace M2M_Private;

    /*
     * This method creates a single textured quad for one of the sides of the mesher's center tile.
     * There are several cases for the output:
     *
     *    If the tile is not full thickness, then both a piece of the ground mesh and a wall will be output.
     *    If the tile is full thickness...
     *       ... if this is NOT a ground tile, and if either the wall goes downward or goes upward and connects to the ground,
     *              then output the wall
     *       ... if this IS a ground tile, then only a downward wall will be drawn, and only if it goes down to another ground tile
     *              (since otherwise, it is the non-ground tile's responsibility to draw a wall upward)
     *
     * */


    const TileInfo &thisTile = mTileNeighborhood.centerTile();
    const TileInfo &otherTile = mTileNeighborhood(QPoint(1, 1) + SideTools::neighborOffset(sideDirection));



    bool isOtherNull = otherTile.isNull();                      // Is the other tile null? (Are we on the edge of the map?)

    bool isThisFullThickness = thisTile.thickness() >= 1;       // Is this tile full thickness?
    bool isOtherFullThickness = otherTile.thickness() >= 1;     // Is the other tile full thickness?

    bool isThisGround = thisTile.isGround();                    // Is this a ground tile?
    bool isEndAtGround;                                         // Is the end connected to ground?

    float startHeight = thisTile.topHeight();                   // The wall's "start" is at the tile's top quad.
    float endHeight;                                            // The end height of the wall (could be ground or otherTile).


    if (!isThisFullThickness) {
        isEndAtGround = true;
        endHeight = thisTile.groundInfo().groundHeight;
    } else {
        if (!isOtherFullThickness) {
            isEndAtGround = true;
            endHeight = otherTile.groundInfo().groundHeight;
        } else {
            isEndAtGround = otherTile.isGround();
            endHeight = otherTile.topHeight();
        }
    }



    bool shouldOutputGround = !isThisFullThickness;
    bool shouldOutputWall = (!isOtherNull || !isThisFullThickness) && (
                    (!isThisGround && (endHeight < startHeight || isEndAtGround))
                ||  (endHeight < startHeight && isEndAtGround)
                );



    // The offset of the tile from the center of its square, in tile coordinates.
    QVector2D tileCenterOffset = thisTile.offsetFromCenter();

    // The center of the tile in XZ coordinates.
    QVector2D xzTileCenter = offset + QVector2D(-tileCenterOffset.x(), tileCenterOffset.y());


    if (shouldOutputWall) {

        float halfThickness = thisTile.thickness() * 0.5;

        // "Away" vector such that when added to the tile's center in XZ coordinates
        // gives the wall's center in XZ coordinates.
        QVector2D away = SideTools::awayDirection(sideDirection) * halfThickness;

        // The point at the center of the wall in XZ coordinates.
        QVector3D sideCenter = QVector3D(
                                    xzTileCenter.x() + away.x(),
                                    (startHeight + endHeight) * 0.5,
                                    xzTileCenter.y() + away.y());

        QVector2D direction = SideTools::awayDirection(sideDirection);
        float height = startHeight - endHeight;
        bool upsideDown = false;

        if (height < 0) {
            height = -height;
            upsideDown = true;
            direction = -direction;
        }


        meshData.addQuad(Quad::makeVerticalQuad(
                             sideCenter,
                             direction,
                             thisTile.thickness(),
                             height,
                             thisTile.sideImage(),
                             thisTile.sideMaterial(),
                             upsideDown));
    }


    if (shouldOutputGround) {
        // The tile is not full thickness, so this wall is responsible for outputting
        // a piece of the ground beneath the tile. The specific piece it outputs depends
        // on its orientation:
        /*
          (Looking from above in 3D space)

              ^ +Z
              |
          <---
          +X

           _ _ _ _ _ _
          |\    N    /|
          | \ _ _ _ / |
          |  |thin |  |
          |W |tile | E|
          |  |_ _ _|  |
          | /       \ |
          |/_ _ S _ _\|

         */

        auto groundImage = thisTile.groundInfo().groundImage;
        auto groundMaterial = thisTile.groundInfo().groundMaterial;

        meshData.addQuad(SideTools::makeTopQuadFragment(sideDirection,
                                                        offset,                 // Center of ground tile in XZ coordinates.
                                                        xzTileCenter,           // Center of tile in XZ coordinates.
                                                        thisTile.thickness(),
                                                        groundImage,
                                                        groundMaterial));
    }
}



/* BEGIN TileInfo */

M2M::TileInfo::TileInfo()
    : mIsNull(true) {}

M2M::TileInfo::TileInfo(const Tile &tile)
    : mIsNull(false)
    , mTopHeight(tile.height())
    , mThickness(tile.thickness())
    , mOffsetFromCenter(tile.position() - QVector2D(0.5, 0.5))
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

        const auto &mat = *tile.material();

        if (!mat.texture().isNull())
            mTopImage = mSideImage = ImageInfo(mat.texture());
        else
            mTopImage = mSideImage = ImageInfo(getDefaultImage());

        mTopMaterial.ambient = mSideMaterial.ambient = mat.ambient();
        mTopMaterial.diffuse = mSideMaterial.diffuse = mat.diffuse();
        mTopMaterial.specular = mSideMaterial.specular = mat.specular();
        mTopMaterial.shininess = mSideMaterial.shininess = mat.shininess();
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
            && offsetFromCenter() == other.offsetFromCenter()
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

    return QSharedPointer<M2M::TileBlockyMesher>::create(*this);
}

/* END TileNeighborhoodInfo */




SharedImageAndSource M2M::TileInfo::DefaultImage = nullptr;
SharedImageAndSource M2M::TileInfo::getDefaultImage()
{
    if (DefaultImage.isNull())
        DefaultImage = ImageAndSource::getSharedImageAndSource("://images/textures/grassTexture.jpg");

    return DefaultImage;
}
