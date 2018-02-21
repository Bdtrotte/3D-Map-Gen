#ifndef M2MTILEMESHER_H
#define M2MTILEMESHER_H


#include <QVector3D>
#include <QSharedPointer>
#include <QImage>

#include "simpletexturedobject.h"


/**
 * @brief A collection of methods for generating tile mesh data.
 */
class M2MTileMesher {
public:

    /**
     * @brief Class to temporarily replace the M2MPropertySet parameter to getTopMesh().
     */
    struct Input {
        float topHeight;
        float baseHeight;
        QSharedPointer<QImage> image;

        bool operator ==(const Input &other) const
        {
            return topHeight == other.topHeight && baseHeight == other.baseHeight && image.data() == other.image.data();
        }

        bool operator !=(const Input &other) const
        {
            return !(*this == other);
        }
    };


    /**
     * @brief Generates a square for the top of the tile. The offset's Y component
     * should not depend on the tile's height because that is considered separately.
     * @param tileProperties    The properties of the tile that is to be converted.
     * @param offset            The position of the center of the tile (at height 0).
     * @param scale             A multiplier. By default, each tile is a 1-by-1 square.
     * @return
     */
    static QSharedPointer<SimpleTexturedObject> getTopMesh(Input tileProperties, QVector3D offset, float scale = 1.0);

private:


    static QSharedPointer<QImage> getTestingImage();

    static QSharedPointer<QImage> TestingImage;
};

#endif // M2MTILEMESHER_H
