#ifndef BLOCKYPOLYGONTILEMESHER_H
#define BLOCKYPOLYGONTILEMESHER_H

#include "abstractpolygontilemesher.h"

namespace M2M {

class BlockyPolygonTileMesher : public AbstractPolygonTileMesher
{
public:
    BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd);

protected:
    QVector<BetterPolygon> topPolygon();
};

}

#endif // BLOCKYPOLYGONTILEMESHER_H
