#ifndef BLOCKYPOLYGONTILEMESHER_H
#define BLOCKYPOLYGONTILEMESHER_H

#include "abstractpolygontilemesher.h"

namespace M2M {

class BlockyPolygonTileMesher : public AbstractPolygonTileMesher
{
public:
    BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd);

protected:
    QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> topPolygons();

private:
    void determinIfWallShouldDrop(QPointF a, QPointF b, float &height, bool &shouldDrop);
};

}

#endif // BLOCKYPOLYGONTILEMESHER_H
