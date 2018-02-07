
// For std::min_element
#include <algorithm>

#include <QTimer>

#include "map2mesh.h"

#include "m2mtilemesher.h"

Map2Mesh::Map2Mesh(TileMap *tileMap, QObject *parent)
    : QObject(parent),
      mTileMap(tileMap)
{
    remakeAll();


    // Connect the tile changed & map resized signals.
    connect(tileMap, &TileMap::tileChanged, this, &Map2Mesh::tileChanged);
    connect(tileMap, &TileMap::resized, this, &Map2Mesh::remakeAll);
}


QVector<QSharedPointer<RenderableObject>> Map2Mesh::getMeshes() const
{
    QVector<QSharedPointer<RenderableObject>> meshes;

    for (QSharedPointer<RenderableObject> tileMesh : mTileMeshes)
        meshes.append(tileMesh);

    return meshes;
}


void Map2Mesh::tileChanged(int x, int y)
{
    if (!mInferScheduled) {
        mInferScheduled = true;

        QTimer::singleShot(500, this, [this] () {
            mInferScheduled = false;
            inferProperties();
        });
    }
}


void Map2Mesh::remakeAll()
{
    mTileMeshes = Array2D<QSharedPointer<RenderableObject>>(mTileMap->mapSize());

    // Reset tile properties to a 0x0 grid so that all meshes are changed in inferProperties.
    mTileProperties = Array2D<M2MPropertySet>();

    inferProperties();
}



// TODO: There may be threading issues here! What if mTileMap changes while we are processing?
void Map2Mesh::inferProperties()
{
    const Array2D<QSharedPointer<Tile>> &grid = mTileMap->getArray2D();

    Array2D<M2MPropertySet> newProperties = Array2D<M2MPropertySet>(mTileMap->mapSize());

    // Set up basic properties (that depend only on vanilla Tile properties).
    for (int x = 0; x < mTileMap->width(); ++x) {
        for (int y = 0; y < mTileMap->height(); ++y) {
            auto props = M2MPropertySet();

            M2MPropertyClass *heights = Map2Mesh::Properties::Height;

            // Base height will be the minimum height of all surrounding tiles.
            auto lowestNeighborItr = std::min_element(
                grid.begin_neighbors(x, y),
                grid.end_neighbors(x, y),
                [](const QSharedPointer<Tile> &t1, const QSharedPointer<Tile> &t2) {
                    return t1->height() < t2->height();
                }
            );

            float minSurroundingHeight = (*lowestNeighborItr)->height();
            props.addProperty(M2MPropertyInstance::createInstance(
                                    heights, {
                                      { heights, "baseHeight", minSurroundingHeight },
                                      { heights, "topHeight", grid(x,y)->height() }
                                    }
                                  )
                              );

            newProperties(x, y) = props;
        }
    }


    // In the future, extra neighbor-based property inference will go here.


    // Remake meshes for all tiles whose properties changed.
    QVector3D center = QVector3D(mTileMap->mapSize().width()/2.0, 0, mTileMap->mapSize().height()/2.0);
    for (int x = 0; x < newProperties.width(); ++x) {
        for (int y = 0; y < newProperties.height(); ++y) {
            if (mTileProperties.size() != newProperties.size() || mTileProperties(x, y) != newProperties(x, y)) {
                mTileMeshes(x, y) = QSharedPointer<RenderableObject>::create(M2MTileMesher::getTopMesh(newProperties(x, y), QVector3D(x, 0, y) - center));
            }
        }
    }

    mTileProperties = newProperties;


    emit mapMeshUpdated();
}

