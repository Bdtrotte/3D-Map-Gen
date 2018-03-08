#include "polygon.h"

Polygon::Polygon()
{

}

Polygon::Polygon(const QVector<QPointF> &points)
{

}

bool Polygon::chordIsClear(int ind1, int ind2) const
{

}

QPair<Polygon, Polygon> Polygon::splitPolygon() const
{

}

QPair<Polygon, Polygon> Polygon::splitPolygon(QPair<int, int> chord) const
{

}

QList<Triplet<QPointF, QPointF, QPointF> > Polygon::triangulate() const
{

}

QVector<Polygon> Polygon::intersected(const Polygon &other) const
{

}
