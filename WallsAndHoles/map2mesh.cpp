#include "map2mesh.h"

#include "m2mtilemesher.h"

Map2Mesh::Map2Mesh(TileMap *tileMap)
    : mTileMap(tileMap)
{
    remakeAll();

    // Connect the tile changed & map resized signals.
    connect(tileMap, &TileMap::tileChanged, this, &Map2Mesh::tileChanged);
    connect(tileMap, &TileMap::resized, this, &Map2Mesh::remakeAll);
}


QVector<QSharedPointer<RenderableObject>> Map2Mesh::getMeshes() const {
    QVector<QSharedPointer<RenderableObject>> meshes;

    for (QSharedPointer<RenderableObject> tileMesh : mTileMeshes)
        meshes.append(tileMesh);

    return tileMesh;
}


void Map2Mesh::tileChanged(int x, int y) {
    QVector3D center = QVector3D(mTileMap->mapSize().width()/2.0, mTileMap->mapSize().height()/2.0);
    mTileMeshes(x, y) = QSharedPointer<RenderableObject>::create(M2MTileMesher::getTopMesh(mTileMap->tileAt(x, y), QVector3D(x, 0, y) - center));
    remakeMesh();
}


void Map2Mesh::remakeAll() {
    mTileMeshes = Array2D<QSharedPointer<RenderableObject>>(mTileMap->mapSize());

    // Create the tile meshes.
    QVector3D center = QVector3D(mTileMap->mapSize().width()/2.0, mTileMap->mapSize().height()/2.0);
    for (int x = 0; x < mTileMap->mapSize().width(); ++x)
        for (int y = 0; y < mTileMap->mapSize().height(); ++y)
            mTileMeshes(x, y) = QSharedPointer<RenderableObject>::create(M2MTileMesher::getTopMesh(mTileMap->tileAt(x, y), QVector3D(x, 0, y) - center));
}


//Map2Mesh::Map2Mesh(const TileMap& tileMap)
//    : mTileProperties(tileMap.mapSize().width(), tileMap.mapSize().height()),
//      mVEdgeProperties(tileMap.mapSize().width()+1, tileMap.mapSize().height()),
//      mHEdgeProperties(tileMap.mapSize().width(), tileMap.mapSize().height()+1),
//      mCornerProperties(tileMap.mapSize().width()+1, tileMap.mapSize().height()+1)
//{

//    /*
//     * Big steps:
//     * 1) Process tiles, edges and corners to compute extra properties.
//     * 2) Based on properties, assign a mesh generator to each corner, edge and tile in a consistent way.
//     * 3) Generate all meshes.
//     * 4) Stitch all meshes.
//     * 5) Combine all vertices and faces.
//     *
//     * 2-1) For each corner, pick the best mesh generator.
//     * 2-2) For each edge, pick the best mesh generator that is compatible with the adjacent corners.
//     * 2-3) For each tile, pick the best mesh generator that is compatible with the adjacent edges and corners.
//     *
//     * 3-1) Call generate() on each mesh generator. This will create a grid of stitchable meshes.
//     *
//     * 4-1)
//     * */



//    // Grid of stitchable meshes. Entries for corners and edges as well as tiles.
//    Array2D<M2MStitchableMesh> meshes;


//    QVector<QVector3D> allVertices;
//    QVector<unsigned int> allTriangles;

//    QVector<QVector<QVector3D>> vertexEquivalenceClasses;


//    QMap<M2MStitchableMesh *, QMap<unsigned int, unsigned int>> vertexToGlobalIndex;
//    QMap<M2MStitchableMesh *, QMap<unsigned int, unsigned int>> vertexToEquivIndex;

//    for (M2MStitchableMesh mesh : meshes) {

//        // Map from local (mesh) indices to global (output) indices.
//        QVector<unsigned int> indexMap;

//        /*
//         * For each vertex:
//         *    if not in interface,
//         *      add straight to global array
//         *    else
//         *      for each interface,
//         *          if any other matching vertex from this interface belongs to a mesh that has been processed,
//         *              1) use the corresponding global index
//         *              2) find the equiv class index, and append to the equiv class
//         *          else
//         *              1) append to global array
//         *              2) create a new equiv class
//         *              3) record global & equiv index
//         *
//         * */
//    }
//}
