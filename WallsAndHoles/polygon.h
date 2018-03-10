#ifndef POLYGON_H
#define POLYGON_H

#include <QVector2D>
#include <QVector>
#include <QLineF>
#include <QRectF>
#include <QPolygonF>

#include "triplet.h"

/**
 * @brief The Polygon class
 *
 * Strict polygon class with floating point precision.
 *
 * Rules of this are:
 *    All points must be unique
 *    No edges may overlap
 *    Points must be counter clockwise
 */
class BetterPolygon
{
public:
    BetterPolygon() {}
    BetterPolygon(const QVector<QPointF> &points);
    BetterPolygon(const QPolygonF &polygon);

    bool isValid() const;

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

    bool connectionIsClear(const BetterPolygon &other, int pointOnThis, int pointOnOther) const;

    bool overlaps(const BetterPolygon &other) const;

    /**
     * @brief splitPolygon
     *
     * split this polygon into two with some chord.
     * The chord is choosen at random.
     *
     * @return
     */
    QPair<BetterPolygon, BetterPolygon> splitPolygon() const;

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
    QPair<BetterPolygon, BetterPolygon> splitPolygon(QPair<int, int> chord) const;

    /**
     * @brief triangulate
     *
     * Returns a list of triangles that represent this polygon
     *
     * @return
     */
    QList<Triplet<QPointF, QPointF, QPointF>> triangulate() const;

    QVector<BetterPolygon> subtract(const BetterPolygon &other) const;
    QVector<BetterPolygon> intersect(const BetterPolygon &other) const;
    QVector<BetterPolygon> united(const BetterPolygon &other) const;

    const QVector<QPointF> &points() const { return mPolygon; }
    QVector<QPointF> &points() { return mPolygon; }

private:
    QPolygonF mPolygon;
};

#endif // POLYGON_H
