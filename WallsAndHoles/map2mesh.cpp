
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
    mTileObjects = Array2D<QVector<QSharedPointer<SimpleTexturedObject>>>(mTileMap->mapSize());
    mScene->clear();

    mTileMeshers = Array2D<QSharedPointer<M2M::TileMesher>>(mTileMap->mapSize());

    inferProperties();
}



// TODO: There may be threading issues here! What if mTileMap changes while we are processing?
void Map2Mesh::inferProperties()
{
    const Array2D<QSharedPointer<Tile>> &grid = mTileMap->getArray2D();

    QVector2D mapCenter(mTileMap->width() * 0.5, mTileMap->height() * 0.5);


    for (int x = 0; x < mTileMap->width(); ++x) {
        for (int y = 0; y < mTileMap->height(); ++y) {

            Array2D<const Tile *> neighborhood(3, 3);

            bool hasLeft = x > 0;
            bool hasRight = x < grid.width()-1;
            bool hasDown = y > 0;
            bool hasUp = y < grid.height()-1;

            neighborhood(0, 0) = hasLeft  && hasDown ? grid(x-1,y-1).data() : nullptr;
            neighborhood(1, 0) =             hasDown ? grid( x ,y-1).data() : nullptr;
            neighborhood(2, 0) = hasRight && hasDown ? grid(x+1,y-1).data() : nullptr;

            neighborhood(0, 1) = hasLeft             ? grid(x-1, y ).data() : nullptr;
            neighborhood(1, 1) =                       grid( x , y ).data();
            neighborhood(2, 1) = hasRight            ? grid(x+1, y ).data() : nullptr;

            neighborhood(0, 2) = hasLeft  && hasUp   ? grid(x-1,y+1).data() : nullptr;
            neighborhood(1, 2) =             hasUp   ? grid( x ,y+1).data() : nullptr;
            neighborhood(2, 2) = hasRight && hasUp   ? grid(x+1,y+1).data() : nullptr;



            auto newMesher = M2M::TileMesher::getMesherForTile(
                        neighborhood,
                        mTileMeshers(x, y).data());


            // If the tile's mesh should be updated, update it.
            if (newMesher != nullptr) {
                auto oldObjects = mTileObjects(x, y);

                for (auto obj : oldObjects)
                    mScene->removeObject(obj);

                auto newObjects = newMesher->makeMesh(QVector2D(x, y) - mapCenter);

                for (auto obj : newObjects)
                    mScene->addObject(obj);

                mTileObjects(x, y) = newObjects;
                mTileMeshers(x, y) = newMesher;
            }

        }
    }


    mScene->commitChanges();
}

