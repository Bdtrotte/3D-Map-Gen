#ifndef TIMAMAP2MESH_H
#define TIMAMAP2MESH_H


#include <QObject>

#include "renderableobject.h"
#include "tilemap.h"

#include "array2d.h"
#include "m2mpropertyclass.h"
#include "m2mpropertyinstance.h"
#include "m2mpropertyset.h"

/**
 * @brief An object that keeps track of map-to-mesh conversion.
 */
class Map2Mesh {

    Q_OBJECT

public:

    /**
     * @brief Creates the conversion object.
     * @param tilemap - The tilemap on which the conversion will happen.
     */
    Map2Mesh(TileMap *tileMap);

    /**
     * @brief Returns a list of all the tile meshes.
     * @return The list of all tile meshes.
     */
    QVector<QSharedPointer<RenderableObject>> getMeshes() const;


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
     * @brief Combines all of the per-tile meshes into one big mesh.
     */
    void remakeMesh();


    TileMap *mTileMap;

    QSharedPointer<RenderableObject> mOutputMesh;

    /**
     * @brief A mesh for every tile.
     */
    Array2D<QSharedPointer<RenderableObject>> mTileMeshes;
};

#endif // TIMAMAP2MESH_H
