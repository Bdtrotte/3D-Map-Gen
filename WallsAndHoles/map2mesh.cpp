#include <QMutexLocker>
#include <QTimer>

#include "map2mesh.h"
#include "array2dtools.h"


Map2Mesh::Map2Mesh(TileMap *tileMap, QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap)
    , mScene(SimpleTexturedScene::makeScene())
    , mSceneUpdateScheduled(false)
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

    for (int x = 0; x < mTileMap->width(); ++x) {
        for (int y = 0; y < mTileMap->height(); ++y) {
            // Get a new mesher for the tile. If this is nullptr,
            // that means that the tile's mesh does not need an update.
            auto newMesher = M2M::AbstractTileMesher::getMesherForTile(mTileMap, QPoint(x, y));

            auto oldObjects = mTileObjects(x, y);

            for (auto obj : oldObjects)
                mScene->removeObject(obj);

            auto newObjects = newMesher->makeMesh(QVector2D(x, y));

            for (auto obj : newObjects)
                mScene->addObject(obj);

            mTileObjects(x, y) = newObjects;
        }
    }

    mTilesToUpdate.clear();

    mScene->commitChanges();
}

