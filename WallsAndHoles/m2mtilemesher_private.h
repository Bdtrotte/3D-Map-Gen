#ifndef M2MTILEMESHER_PRIVATE_H
#define M2MTILEMESHER_PRIVATE_H

#include <QPoint>
#include <QVector2D>
#include <QVector3D>

#include "m2mpartialmesh.h"

/*
 * Various helper functions for the M2M tile mesher process.
 * */


namespace M2M_Private {

enum SideDirection {
    NORTH, EAST, SOUTH, WEST
};

/**
 * @brief polygonMesh
 * @param polygon
 * @param height
 * @return
 */
M2M::PartialMeshData polygonMesh(const QPolygonF &polygon,
                                 float height,
                                 M2M::ImageInfo image,
                                 M2M::PhongInfo material);

/**
 * @brief polygonSidesMesh
 * @param polygon
 * @param edgesToDrop
 * @param topHeight
 * @param bottomHeight
 * @return
 */
M2M::PartialMeshData polygonSidesMesh(QPolygonF polygon,
                                      QVector<bool> edgesToDrop,
                                      QVector<float> bottomHeight,
                                      float topHeight,
                                      M2M::ImageInfo image,
                                      M2M::PhongInfo material);

namespace SideTools {
    /**
     * @brief neighborOffset Computes the offset for the coordinates of a neighbor of a tile.
     * @param side           The direction towards the neighbor.
     * @return  NORTH -> (0, -1)
     *          EAST  -> (1, 0)
     *          SOUTH -> (0, -1)
     *          WEST  -> (-1, 0)
     */
    QPoint neighborOffset(SideDirection side);

    /**
     * @brief awayDirection Computes the "away" direction for a side on a tile, in XZ coordinates.
     *
     * @param side  The direction a side is facing.
     * @return  NORTH -> (0, -1)
     *          EAST  -> (1, 0)
     *          SOUTH -> (0, 1)
     *          WEST  -> (-1, 0)
     */
    QVector2D awayDirection(SideDirection side);

    /**
     * @brief normalDirection Computes the normal direction for a vertical side of a tile.
     *
     * @param side  The direction a side is facing.
     * @return  NORTH -> (0, 0, -1)
     *          EAST  -> (1, 0, 0)
     *          SOUTH -> (0, 0, 1)
     *          WEST  -> (-1, 0, 0)
     */
    QVector3D normalDirection(SideDirection side);


    /**
     * @brief makeTopQuadFragment   Creates a fragment of a top-facing quad.
     * @param side                  Which fragment? (there are 4!)
     * @param quadCenter            Center in XZ coordinates of the quad. The quad is assumed to be size (1, 1).
     * @param minusQuadCenter       Center in XZ coordinates of the quad that is subtracted.
     * @param minusQuadSize         Size of the subtracted quad (so it is assumed a square).
     * @param quadImage             Texture for the main quad.
     * @param quadMaterial          Material for the main quad.
     * @return                      TODO
     *
     * TODO: Preconditions.
     */
    M2M::Quad makeTopQuadFragment(SideDirection side,
                                  QVector2D quadCenter,
                                  QVector2D minusQuadCenter,
                                  float minusQuadSize,
                                  M2M::ImageInfo quadImage,
                                  M2M::PhongInfo quadMaterial);
}
}


#endif // M2MTILEMESHER_PRIVATE_H
