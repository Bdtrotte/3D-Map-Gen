#include "blockypolygontilemesher.h"

using namespace M2M;

BlockyPolygonTileMesher::BlockyPolygonTileMesher(TileNeighborhoodInfo nbhd)
    : AbstractPolygonTileMesher(nbhd) {}

QVector<BetterPolygon> BlockyPolygonTileMesher::topPolygon()
{
    return {};
}
