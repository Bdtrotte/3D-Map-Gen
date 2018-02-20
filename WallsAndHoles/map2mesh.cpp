
// For std::min_element
#include <algorithm>

#include <QTimer>

#include "map2mesh.h"

Map2Mesh::Map2Mesh(TileMap *tileMap, QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap)
    , mScene(SimpleTexturedScene::makeScene())
{
    if (mTileMap) {
        remakeAll();


        // Connect the tile changed & map resized signals.
        connect(mTileMap, &TileMap::tileChanged, this, &Map2Mesh::tileChanged);
        connect(mTileMap, &TileMap::resized, this, &Map2Mesh::remakeAll);
    }
}


SharedSimpleTexturedScene Map2Mesh::getScene() const
{
    return mScene;
}

void Map2Mesh::tileChanged(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

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
    mTileMeshes = Array2D<QSharedPointer<SimpleTexturedObject>>(mTileMap->mapSize());
    mScene->clear();

    // Reset tile properties to a 0x0 grid so that all meshes are changed in inferProperties.
    mTileProperties = Array2D<M2MTileMesher::Input>();

    inferProperties();
}



// TODO: There may be threading issues here! What if mTileMap changes while we are processing?
void Map2Mesh::inferProperties()
{
    const Array2D<QSharedPointer<Tile>> &grid = mTileMap->getArray2D();

    Array2D<M2MTileMesher::Input> newProperties = Array2D<M2MTileMesher::Input>(mTileMap->mapSize());

    // Collect basic properties (like the base height of a tile's mesh).
    for (int x = 0; x < mTileMap->width(); ++x) {
        for (int y = 0; y < mTileMap->height(); ++y) {

            // Base height will be the minimum height of all surrounding tiles.
            auto lowestNeighborItr = std::min_element(
                grid.begin_neighbors(x, y),
                grid.end_neighbors(x, y),
                [](const QSharedPointer<Tile> &t1, const QSharedPointer<Tile> &t2) {
                    return t1->height() < t2->height();
                }
            );

			float minSurroundingHeight = grid(x, y)->height();
			if (lowestNeighborItr != grid.end_neighbors(x, y))
            	minSurroundingHeight = std::min((*lowestNeighborItr)->height(), grid(x, y)->height());


            float baseHeight = minSurroundingHeight;
            float topHeight = grid(x,y)->height();

            QSharedPointer<QImage> image =
                    grid(x,y)->hasTileTemplate() ?
                        grid(x,y)->material().getTexture()
                      : nullptr;

            newProperties(x, y) = {
                    topHeight,
                    baseHeight,
                    image
            };
        }
    }


    // In the future, extra neighbor-based property inference will go here.

    bool sizeChanged = mTileProperties.size() != newProperties.size();

    // Remake meshes for all tiles whose properties changed.
    QVector3D center = QVector3D(mTileMap->mapSize().width()/2.0, 0, mTileMap->mapSize().height()/2.0);
    for (int x = 0; x < newProperties.width(); ++x) {
        for (int y = 0; y < newProperties.height(); ++y) {
            if (sizeChanged || mTileProperties(x, y) != newProperties(x, y)) {
                auto oldObj = mTileMeshes(x, y);
                auto newObj = M2MTileMesher::getTopMesh(newProperties(x, y), QVector3D(x, 0, y) - center);
                mTileMeshes(x, y) = newObj;

                if (oldObj != nullptr)
                    mScene->removeObject(oldObj);

                mScene->addObject(newObj);
            }
        }
    }

    mTileProperties = newProperties;


    mScene->commitChanges();
}

