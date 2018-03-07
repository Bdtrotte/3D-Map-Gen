#include <QMutexLocker>
#include <QTimer>

#include "map2mesh.h"
#include "array2dtools.h"


Map2Mesh::Map2Mesh(TileMap *tileMap, QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap)
    , mScene(SimpleTexturedScene::makeScene())
{
    if (mTileMap) {
        // This will set up and initialize all output-related variables.
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
    // Update this tile and its neighboring tiles.
    QMutexLocker sceneLocker(&mSceneUpdateMutex);
    mTilesToUpdate += {x, y};
    mTilesToUpdate += getValidNeighbors(x, y, mTileMap->mapSize());
    sceneLocker.unlock();


    if (!mSceneUpdateScheduled) {
        mSceneUpdateScheduled = true;

        QTimer::singleShot(500, this, [this] () {
            mSceneUpdateScheduled = false;
            updateScene();
        });
    }
}


void Map2Mesh::remakeAll()
{
    mTileObjects = TileObjectGrid(mTileMap->mapSize());
    mTileMeshers = TileMesherGrid(mTileMap->mapSize());
    mScene->clear();

    // Update all points.
    QMutexLocker locker(&mSceneUpdateMutex);
    for (const QPoint &pt : mTileMap->getArray2D().indices())
        mTilesToUpdate.insert(pt);
    locker.unlock();


    updateScene();
}


void Map2Mesh::updateScene()
{
    QMutexLocker locker(&mSceneUpdateMutex);


    // Using the grid representation of mTileMap for convenience.
    const Array2D<QSharedPointer<Tile>> &grid = mTileMap->getArray2D();

    // Update every point that needs updating.
    for (const QPoint &point : mTilesToUpdate) {
        int x = point.x();
        int y = point.y();


        /*
         * Compute the 3x3 neighborhood of the point.
         * The point should be at (1, 1), and nonexistent neighbors should be nullptr.
         * */
        Array2D<const Tile *> neighborhood(3, 3, nullptr);

        neighborhood(1, 1) = grid(x, y).data();
        for (const QPoint &neighbor : getValidNeighbors(point, mTileMap->mapSize()))
            neighborhood(neighbor - point + QPoint(1, 1)) = grid(neighbor).data();


        // Get a new mesher for the tile. If this is nullptr,
        // that means that the tile's mesh does not need an update.
        auto newMesher = M2M::AbstractTileMesher::getMesherForTile(
                    neighborhood,
                    mTileMeshers(x, y).data());


        // If the tile's mesh should be updated, update it.
        if (newMesher != nullptr) {

            /*
             * NOTE: The way this is currently set up, a TileMesher will not be
             * able to keep and modify a reference to an object. This part of
             * the code may be subject to change later (although currently, it
             * is sufficiently efficient).
             * */


            auto oldObjects = mTileObjects(x, y);

            for (auto obj : oldObjects)
                mScene->removeObject(obj);

            auto newObjects = newMesher->makeMesh(QVector2D(x, y));

            for (auto obj : newObjects)
                mScene->addObject(obj);

            mTileObjects(x, y) = newObjects;
            mTileMeshers(x, y) = newMesher;
        }
    }


    mTilesToUpdate.clear();

    mScene->commitChanges();
}

