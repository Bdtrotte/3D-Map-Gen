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
