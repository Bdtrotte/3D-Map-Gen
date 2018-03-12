#ifndef GROUNDBLOCKYPOLYGONTILEMESHER_H
#define GROUNDBLOCKYPOLYGONTILEMESHER_H

#include "abstractpolygontilemesher.h"

namespace M2M {

class GroundBlockyPolygonTileMesher : public AbstractPolygonTileMesher
{
public:
    GroundBlockyPolygonTileMesher(TileNeighborhoodInfo nbhd);

protected:
    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> topPolygons(QVector<const Tile *> *heightAndMaterial);
};

}

#endif // GROUNDBLOCKYPOLYGONTILEMESHER_H
