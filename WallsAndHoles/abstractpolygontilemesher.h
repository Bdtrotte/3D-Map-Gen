#ifndef POLYGONMESHMAKER_H
#define POLYGONMESHMAKER_H

#include "m2mtilemesher.h"
#include "polygon.h"
#include "m2mpartialmesh.h"

namespace M2M {

class AbstractPolygonTileMesher : public AbstractTileMesher
{
public:
    AbstractPolygonTileMesher(TileNeighborhoodInfo nbhd);

    QVector<QSharedPointer<SimpleTexturedObject>> makeMesh(QVector2D offset);

protected:
    //Justification of heightAndMAterial:
    //Some meshers may want to not use this tiles height and material, so the mesher can handle an optional vector of tiles
    //The vector must either be left empty, or filled to equal the size of the main return vector
    virtual QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> topPolygons(QVector<const Tile *> *heightAndMaterial) = 0;

private:
    PartialMeshData makeTop(const BetterPolygon &polygon,
                            float height,
                            const TileMaterial *material) const;
    PartialMeshData makeSide(const BetterPolygon &polygon,
                             float startHegiht,
                             const QVector<float> endHeight,
                             const QVector<bool> dropWall,
                             const TileMaterial *material) const;
};

}

#endif // POLYGONMESHMAKER_H
