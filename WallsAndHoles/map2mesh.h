#ifndef TIMAMAP2MESH_H
#define TIMAMAP2MESH_H


#include <QObject>

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
     * @brief Figures out mTileProperties for all tiles, and updates meshes when
     * properties change.
     */
    void inferProperties();


    /**
     * @brief The TileMap that is the input to this Map2Mesh object.
     */
    TileMap *mTileMap;


    /**
     * @brief The Scene which contains the output of the Map2Mesh object.
     */
    SharedSimpleTexturedScene mScene;


    /**
     * @brief Mesh data for every tile. A tile's mesh may consist of multiple objects.
     */
    Array2D<QVector<QSharedPointer<SimpleTexturedObject>>> mTileObjects;

    /**
     * @brief A mesher for every tile. These are also used to determine
     * whether a tile has changed.
     */
    Array2D<QSharedPointer<M2M::TileMesher>> mTileMeshers;


    /**
     * @brief Whether an inferProperties() call has been scheduled. Used in tileChanged().
     */
    bool mInferScheduled;

public:
    struct Properties {
        static M2MPropertyClass *Height;
    };
};


#endif // TIMAMAP2MESH_H
