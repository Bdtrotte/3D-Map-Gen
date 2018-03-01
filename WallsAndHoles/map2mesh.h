#ifndef TIMAMAP2MESH_H
#define TIMAMAP2MESH_H


#include <QObject>
#include <QSet>
#include <QMutex>

#include "simpletexturedscene.h"
#include "simpletexturedobject.h"

#include "tilemap.h"

#include "m2mpropertyclass.h"
#include "m2mtilemesher.h"

#include "array2d.h"

/**
 * @brief An object that keeps track of map-to-mesh conversion.
 */
class Map2Mesh : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Creates the conversion object.
     * @param tilemap - The tilemap on which the conversion will happen.
     */
    Map2Mesh(TileMap *tileMap, QObject *parent = nullptr);


    /**
     * @brief Returns the Scene that this Map2Mesh instance works with.
     */
    SharedSimpleTexturedScene getScene() const;

public slots:
    /**
     * @brief Modifies the mesh near the tile that changed.
     * @param x
     * @param y
     */
    void tileChanged(int x, int y);

    /**
     * @brief Completely remakes all tile meshes.
     */
    void remakeAll();


protected:
    /**
     * @brief Updates the scene for all tiles that need updates.
     */
    void updateScene();


    /**
     * @brief The TileMap that is the input to this Map2Mesh object.
     */
    TileMap *mTileMap;


    /**
     * @brief The Scene which contains the output of the Map2Mesh object.
     */
    SharedSimpleTexturedScene mScene;


    /// A grid containing lists of objects. A tile's "mesh" may consist of several
    /// objects for texturing purposes.
    using TileObjectGrid = Array2D<QVector<QSharedPointer<SimpleTexturedObject>>>;

    /// A grid containing TileMeshers. These act like "tokens" to check whether
    /// a tile's mesh *really* needs to be updated. In addition, they allow for
    /// reuse---a TileMesher may keep some reference to its last output, and
    /// then modify that instead of creating new output.
    using TileMesherGrid = Array2D<QSharedPointer<M2M::TileMesher>>;


    /**
     * @brief Mesh data for every tile.
     */
    TileObjectGrid mTileObjects;

    /**
     * @brief A mesher for every tile.
     */
    TileMesherGrid mTileMeshers;


    /**
     * @brief Whether an updateScene() call has been scheduled. Used in tileChanged().
     */
    bool mSceneUpdateScheduled;

    /**
     * @brief Coordinates of tiles that need updating.
     */
    QSet<QPoint> mTilesToUpdate;

    /**
     * @brief Mutex for scene-update related operations.
     */
    QMutex mSceneUpdateMutex;


public:
    struct Properties {
        static M2MPropertyClass *Height;
    };
};


#endif // TIMAMAP2MESH_H
