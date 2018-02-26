#include "m2mtilemesher.h"
#include "map2mesh.h"



QSharedPointer<M2M::TileMesher> M2M::TileMesher::getMesherForTile(
        Array2D<const Tile *> neighborhood,
        const TileMesher *oldMesher)
{
    TileNeighborhoodInfo newNeighborhood(neighborhood);


    // If the neighborhood didn't change, don't supply a new mesher.
    if (oldMesher != nullptr && newNeighborhood == oldMesher->mTileNeighborhood)
        return nullptr;

    // If the neighborhood changed, supply a new mesher that can properly mesh it.
    else
        return newNeighborhood.makeMesher();
}


M2M::TileMesher::TileMesher(TileNeighborhoodInfo nbhd)
    : mTileNeighborhood(nbhd) {}




M2M::TileBlockyMesher::TileBlockyMesher(TileNeighborhoodInfo nbhd)
    : M2M::TileMesher(nbhd) {}


QVector<QSharedPointer<SimpleTexturedObject>> M2M::TileBlockyMesher::makeMesh(QVector2D offset)
{
    PartialMeshData mesh;

    makeTopMesh(mesh, offset);

    makeVerticalSideMesh(mesh, Direction::NORTH, offset);
    makeVerticalSideMesh(mesh, Direction::EAST, offset);
    makeVerticalSideMesh(mesh, Direction::SOUTH, offset);
    makeVerticalSideMesh(mesh, Direction::WEST, offset);

    return mesh.constructObjects();
}





void M2M::TileMesher::makeTopMesh(PartialMeshData &meshData, QVector2D offset)
{
    /*
     * This method just creates a single textured quad for the mesher's center tile.
     * */

    QVector2D t1(0, 1);
    QVector2D t2(1, 1);
    QVector2D t3(1, 0);
    QVector2D t4(0, 0);

    QVector2D offsetFromCenter = mTileNeighborhood.centerTile().offsetFromCenter();
    float thickness = mTileNeighborhood.centerTile().thickness();
    float topHeight = mTileNeighborhood.centerTile().topHeight();

    QVector3D center(offsetFromCenter.x() + offset.x(), topHeight, offsetFromCenter.y() + offset.y());
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


void M2M::TileMesher::makeVerticalSideMesh(PartialMeshData &meshData, Direction sideDirection, QVector2D offset)
{
    /*
     * This method creates a single textured quad for one of the sides of the mesher's center tile.
     * There are several cases for the output:
     *  If the center tile and the other tile are both full thickness..
     *    ..and the other tile is higher up, output something ONLY IF the other tile is ground (else output nothing).
     *    ..and the other tile is lower:
     *          - if center isn't ground OR both center and other are ground,
     *              output a vertical wall going down to the other tile.
     *          - else, output nothing
     *
     *  If one of them is not full thickness, output a vertical wall to the ground.
     *
     *  If the center tile is not full thickness, also draw part of the ground mesh.
     *
     *
     *
     * Conditions: centerFull, otherFull, centerGround, otherGround, centerAbove, centerAtGround
     *
     *
     * shouldOutputGround = ~centerFull
     * shouldOutputWall   = endHeight < startHeight ?
     *                          true
     *                        : isEndAtGround && ~isThisGround;
     *
     * if not centerFull:
     *    output ground
     *    if not centerAtGround:
     *       output wall
     * else:
     *    if not centerAtGround:
     *       if otherGround or centerAbove:
     *          output wall
     *    else:
     *       if otherGround and centerAbove:
     *          output wall
     *
     * */

    /* Texture coordinate mapping:
     *
     * t4 t3
     * t1 t2
     * */
    QVector2D t1(0, 1);
    QVector2D t2(1, 1);
    QVector2D t3(1, 0);
    QVector2D t4(0, 0);


    float endHeight;    // "End" is where the side mesh connects to something else (ground or another tile).
    float startHeight;  // "Start" is where the side mesh is connected to the top mesh of the tile.
    float groundHeight; // The height of the ground under this tile.


    const TileInfo &thisTile = mTileNeighborhood.centerTile();
    const TileInfo *otherTile;  // c++ doesn't allow leaving reference variables uninitialized

    bool isOtherNull;           // is there actually a tile on the other side?

    bool isThisGround;          // whether this tile is a ground tile
    bool isEndAtGround;         // whether the end of the wall is on the ground

    bool isThisFullThickness;   // whether this tile has full thickness
    bool isOtherFullThickness;  // whether the other tile has full thickness

    bool shouldOutputGround;    // whether a piece of the ground should be output
    bool shouldOutputWall;      // whether a vertical wall should be output


    /*
     * If a quad's vertices are ordered as follows:
     *      4   3
     *
     *      1   2
     *
     * Then the quad's edges are ordered as follows:
     *        3
     *      4   2
     *        1
     *
     *
     * Vertices 3 & 4 should be connected to the center tile, and vertices 1 & 2 are for the other tile (or ground).
     * */
    QVector3D sideCenter;   // The 3D center of the side mesh.
    QVector3D horz;         // Vector such that sideCenter + horz is on the 4th edge of the quad (see above).
    QVector3D vert;         // Vector such that sideCenter + vert is on the 3rd edge of the quad.

    QVector3D normal;


    // Determine above variables.
    startHeight = thisTile.topHeight();
    groundHeight = thisTile.groundInfo().groundHeight;


    float halfThickness = mTileNeighborhood.centerTile().thickness() * 0.5;
    QVector2D away;
    switch (sideDirection) {
    case NORTH:
        otherTile = &mTileNeighborhood(1, 2);

        away = QVector2D(0, halfThickness);
        normal = QVector3D(0, 0, 1);

        break;
    case EAST:
        otherTile = &mTileNeighborhood(2, 1);

        away = QVector2D(halfThickness, 0);
        normal = QVector3D(1, 0, 0);

        break;
    case SOUTH:
        otherTile = &mTileNeighborhood(1, 0);

        away = QVector2D(0, -halfThickness);
        normal = QVector3D(0, 0, -1);

        break;
    case WEST:
        otherTile = &mTileNeighborhood(0, 1);

        away = QVector2D(-halfThickness, 0);
        normal = QVector3D(-1, 0, 0);

        break;
    }

    isOtherNull = otherTile->isNull();

    isThisFullThickness = thisTile.thickness() >= 1;
    isOtherFullThickness = otherTile->thickness() >= 1;

    isThisGround = thisTile.isGround();

    if (!isThisFullThickness) {
        isEndAtGround = true;
        endHeight = groundHeight;
    } else {
        if (!isOtherFullThickness) {
            isEndAtGround = true;
            endHeight = otherTile->groundInfo().groundHeight;
        } else {
            isEndAtGround = otherTile->isGround();
            endHeight = otherTile->topHeight();
        }
    }



    shouldOutputGround = !isThisFullThickness;
    shouldOutputWall = !isOtherNull && (
                    (!isThisGround && (endHeight < startHeight || isEndAtGround))
                ||  (endHeight < startHeight && isEndAtGround)
                );



    QVector2D tileCenterOffset = thisTile.offsetFromCenter();
    sideCenter = QVector3D(
                offset.x() + tileCenterOffset.x() + away.x(),
                (startHeight + endHeight) * 0.5,
                offset.y() + tileCenterOffset.y() + away.y());
    horz = QVector3D(away.y(), 0, -away.x());
    vert = QVector3D(0, (startHeight - endHeight) * 0.5, 0);


    if (shouldOutputWall) {
        meshData.addQuad(Quad(normal,
                              mTileNeighborhood.centerTile().sideImage(),
                              mTileNeighborhood.centerTile().sideMaterial(),
                              sideCenter + horz - vert, t1,
                              sideCenter + horz + vert, t4,
                              sideCenter - horz + vert, t3,
                              sideCenter - horz - vert, t2));
    }


    if (shouldOutputGround) {
        QVector3D gv1 = sideCenter - horz - vert;
        QVector3D gv2 = sideCenter + horz - vert;
        // gv3 and gv4 are at the tile grid corners
        QVector3D gv3;
        QVector3D gv4;

        // gt1 and gt2 are somewhere inside the ground texture
        QVector2D gt1 = tileCenterOffset + away + QVector2D(-away.y(), away.x()) + QVector2D(0.5, 0.5);
        QVector2D gt2 = tileCenterOffset + away + QVector2D(away.y(), -away.x()) + QVector2D(0.5, 0.5);
        // gt3 and gt4 are at the corners of the ground texture
        QVector2D gt3;
        QVector2D gt4;


        switch (sideDirection) {
        case NORTH:
            gv3 = QVector3D(offset.x() + 0.5, groundHeight, offset.y() + 0.5);
            gv4 = QVector3D(offset.x() - 0.5, groundHeight, offset.y() + 0.5);
            gt3 = QVector2D(1, 0);
            gt4 = QVector2D(0, 0);
            break;
        case EAST:
            gv3 = QVector3D(offset.x() + 0.5, groundHeight, offset.y() - 0.5);
            gv4 = QVector3D(offset.x() + 0.5, groundHeight, offset.y() + 0.5);
            gt3 = QVector2D(1, 1);
            gt4 = QVector2D(1, 0);
            break;
        case SOUTH:
            gv3 = QVector3D(offset.x() - 0.5, groundHeight, offset.y() - 0.5);
            gv4 = QVector3D(offset.x() + 0.5, groundHeight, offset.y() - 0.5);
            gt3 = QVector2D(0, 1);
            gt4 = QVector2D(1, 1);
            break;
        case WEST:
            gv3 = QVector3D(offset.x() - 0.5, groundHeight, offset.y() + 0.5);
            gv4 = QVector3D(offset.x() - 0.5, groundHeight, offset.y() - 0.5);
            gt3 = QVector2D(0, 0);
            gt4 = QVector2D(0, 1);
            break;
        }


        auto groundImage = mTileNeighborhood.centerTile().groundInfo().groundImage;
        auto groundMaterial = mTileNeighborhood.centerTile().groundInfo().groundMaterial;

        // Output part of ground mesh.
        meshData.addQuad(Quad(QVector3D(0, 1, 0),
                              groundImage,
                              groundMaterial,
                              gv1, gt1,
                              gv4, gt4,
                              gv3, gt3,
                              gv2, gt2));
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
    //   Handle separate top / side textures.
    //   Get ground properties from somewhere.


    mGround.groundHeight = 0;
    mGround.groundMaterial.ambient = 1;
    mGround.groundMaterial.diffuse = 1;
    mGround.groundMaterial.specular = 1;
    mGround.groundMaterial.shininess = 1;
    mGround.groundImage.image = getDefaultImage();


    if (tile.hasTileTemplate()) {
        mIsGround = false;

        const auto &mat = *tile.material();

        if (!mat.texture().isNull())
            mTopImage.image = mSideImage.image = mat.texture();
        else
            mTopImage.image = mSideImage.image = getDefaultImage();

        mTopMaterial.ambient = mSideMaterial.ambient = mat.ambient();
        mTopMaterial.diffuse = mSideMaterial.diffuse = mat.diffuse();
        mTopMaterial.specular = mSideMaterial.specular = mat.specular();
        mTopMaterial.shininess = mSideMaterial.shininess = mat.shininess();
    } else {
        mIsGround = true;

        mTopImage.image = mSideImage.image = getDefaultImage();

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
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (nbhd(x, y) == nullptr)
                mTileInfos(x, y) = TileInfo();
            else {
                const Tile &tile = *nbhd(x, y);

                mTileInfos(x, y) = TileInfo(tile);
            }
        }
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

const M2M::TileInfo &M2M::TileNeighborhoodInfo::centerTile() const
{
    return mTileInfos(1, 1);
}

QSharedPointer<M2M::TileMesher> M2M::TileNeighborhoodInfo::makeMesher() const
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
        DefaultImage = ImageAndSource::getSharedImageAndSource(":/textures/exampleTexture.png");

    return DefaultImage;
}
