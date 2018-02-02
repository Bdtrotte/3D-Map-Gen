#ifndef M2MTILEMESHER_H
#define M2MTILEMESHER_H


#include <QVector3D>

#include "m2mpropertyset.h"
#include "renderableobject.h"


/**
 * @brief A collection of methods for generating tile mesh data.
 */
class M2MTileMesher {
public:
    /**
     * @brief Generates a square for the top of the tile. The offset's Y component
     * should not depend on the tile's height because that is considered separately.
     * @param tileProperties    The properties of the tile that is to be converted.
     * @param offset            The position of the center of the tile (at height 0).
     * @param scale             A multiplier. By default, each tile is a 1-by-1 square.
     * @return
     */
    static RenderableObject getTopMesh(const M2MPropertySet &tileProperties, QVector3D offset, float scale = 1.0);
};

#endif // M2MTILEMESHER_H
