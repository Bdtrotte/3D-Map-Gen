#ifndef TIMAMAP2MESH_H
#define TIMAMAP2MESH_H


#include <QObject>

#include "renderableobject.h"
#include "tilemap.h"

#include "m2mpropertyset.h"

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


signals:

    /**
     * @brief Emitted when the output mesh is updated.
     */
    void mapMeshUpdated();

protected:

    /**
     * @brief Figures out mTileProperties for all tiles, and updates meshes when
     * properties change.
     */
    void inferProperties();


    TileMap *mTileMap;

    QSharedPointer<RenderableObject> mOutputMesh;

    /**
     * @brief A mesh for every tile.
     */
    Array2D<QSharedPointer<RenderableObject>> mTileMeshes;

    /**
     * @brief Inferred properties for every tile.
     */
    Array2D<M2MPropertySet> mTileProperties;


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
