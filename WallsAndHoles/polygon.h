#ifndef POLYGON_H
#define POLYGON_H

#include <QVector2D>
#include <QVector>

#include "triplet.h"

class Polygon
{
public:
    Polygon();
    Polygon(const QVector<QPointF> &points);

    /**
     * @brief chordIsClear
     *
     * Returns true if the chord between ind1 and ind2 do not intersect anypart of the polygon
     *
     * @param ind1
     * @param ind2
     * @return
     */
    bool chordIsClear(int ind1, int ind2) const;

    /**
     * @brief splitPolygon
     *
     * split this polygon into two with some chord.
     * The chord is choosen at random.
     *
     * @return
     */
    QPair<Polygon, Polygon> splitPolygon() const;

    /**
     * @brief splitPolygon
     *
     * Attempt to split the polygon at a given chord.
     * If the chord is not clear, this will be returned
     * as the first Polygon, and the second will be empty.
     *
     * @param chord
     * @return
     */
    QPair<Polygon, Polygon> splitPolygon(QPair<int, int> chord) const;

    /**
     * @brief triangulate
     *
     * Returns a list of triangles that represent this polygon
     *
     * @return
     */
    QList<Triplet<QPointF, QPointF, QPointF>> triangulate() const;

    QVector<Polygon> intersected(const Polygon &other) const;

    const QPointF &at(int i) const { return mPoints[i]; }

private:
    QVector<QPointF> mPoints;
};

#endif // POLYGON_H
