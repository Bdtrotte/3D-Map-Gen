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
    virtual QVector<BetterPolygon> topPolygon() = 0;

private:
    PartialMeshData makeTop(const BetterPolygon &polygon, float height, bool ground = false) const;
    PartialMeshData makeSide(const BetterPolygon &polygon, float height, const QPointF &offset) const;
};

}

#endif // POLYGONMESHMAKER_H
