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
    virtual QVector<Triplet<BetterPolygon, QVector<float>, QVector<bool>>> topPolygons() = 0;

private:
    PartialMeshData makeTop(const BetterPolygon &polygon, float height, bool ground = false) const;
    PartialMeshData makeSide(const BetterPolygon &polygon, float startHegiht, const QVector<float> endHeight, const QVector<bool> dropWall) const;
};

}

#endif // POLYGONMESHMAKER_H
