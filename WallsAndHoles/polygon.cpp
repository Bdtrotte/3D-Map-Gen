#include "polygon.h"

#include "geometry.h"

#include <QSet>
#include <QDebug>

uint qHash(const QPointF &key)
{
    return qHash(QPair<float, float>(key.x(), key.y()));
}

BetterPolygon::BetterPolygon(const QVector<QPointF> &points)
{
    mPolygon = points;

    if (mPolygon.isClosed()) mPolygon.removeLast();
}

BetterPolygon::BetterPolygon(const QPolygonF &polygon)
{
    mPolygon = polygon;

    if (mPolygon.isClosed()) mPolygon.removeLast();
}

bool BetterPolygon::isValid() const
{
    if (mPolygon.size() < 3) return false;

    QSet<QPointF> points;

    //Check to see if any points are the same
    for (QPointF p : mPolygon) {
        if (points.contains(p))
            return false;
        points.insert(p);
    }

    //Check to see if any edges cross
    for (int i = 0; i < mPolygon.size(); ++i) {
        int j = (i + 1) % mPolygon.size();

        QLineF lineA(mPolygon[i], mPolygon[j]);

        for (int a = 0; a < mPolygon.size(); ++a) {
            int b = (a + 1) % mPolygon.size();

            if (i == a || i == b || j == a || j == b) continue;

            if (lineA.intersect(QLineF(mPolygon[a], mPolygon[b]), nullptr) == QLineF::BoundedIntersection)
                return false;
        }
    }

    // TODO need to ensure it's counter clockwise

    return true;
}

bool BetterPolygon::chordIsClear(int ind1, int ind2) const
{
    if (ind2 == ((ind1 + 1) % mPolygon.size())
            || ind1 == ((ind2 + 1) % mPolygon.size()))
        return false;

    QLineF line(mPolygon[ind1], mPolygon[ind2]);

    if (!mPolygon.containsPoint(line.center(), Qt::OddEvenFill)) return false;

    for (int i = 0; i < mPolygon.size(); ++i) {
        int j = (i + 1) % mPolygon.size();

        if (i == ind1 || i == ind2 || j == ind1 || j == ind2) continue;

        if (line.intersect(QLineF(mPolygon[i], mPolygon[j]), nullptr) == QLineF::BoundedIntersection)
            return false;
    }

    return true;
}

bool BetterPolygon::connectionIsClear(const BetterPolygon &other, int pointOnThis, int pointOnOther) const
{
    QLineF line(mPolygon[pointOnThis], other.mPolygon[pointOnOther]);

    for (int i = 0; i < mPolygon.size(); ++i) {
        int j = (i + 1) % mPolygon.size();

        if (j == pointOnThis || i == pointOnThis) continue;

        if (line.intersect(QLineF(mPolygon[i], mPolygon[j]), nullptr) == QLineF::BoundedIntersection)
            return false;
    }

    for (int i = 0; i < other.mPolygon.size(); ++i) {
        int j = (i + 1) % other.mPolygon.size();

        if (j == pointOnOther || i == pointOnOther) continue;

        if (line.intersect(QLineF(other.mPolygon[i], other.mPolygon[j]), nullptr) == QLineF::BoundedIntersection)
            return false;
    }

    return true;
}

bool BetterPolygon::overlaps(const BetterPolygon &other) const
{
    for (int i = 0; i < mPolygon.size(); ++i) {
        int j = (i + 1) % mPolygon.size();
        QLineF line(mPolygon[i], mPolygon[j]);

        for (int a = 0; a < other.mPolygon.size(); ++a) {
            int b = (a + 1) % other.mPolygon.size();

            if (line.intersect(QLineF(other.mPolygon[a], other.mPolygon[b]), nullptr) == QLineF::BoundedIntersection)
                return true;
        }
    }

    return false;
}

QPair<BetterPolygon, BetterPolygon> BetterPolygon::splitPolygon() const
{
    //go through all chords, and choose the first that is clear

    for (int i = 0; i < mPolygon.size(); ++i) {
        for (int j = i + 2; j < mPolygon.size(); ++j) {
            if (chordIsClear(i, j)) {
                QVector<QPointF> p1, p2;

                int progress = i;
                while (true) {
                    if (progress == j) {
                        p1.append(mPolygon[progress]);
                        break;
                    }

                    p1.append(mPolygon[progress]);
                    progress = (progress + 1) % mPolygon.size();
                }
                while (true) {
                    if (progress == i) {
                        p2.append(mPolygon[progress]);
                        break;
                    }

                    p2.append(mPolygon[progress]);
                    progress = (progress + 1) % mPolygon.size();
                }

                return QPair<BetterPolygon, BetterPolygon>(p1, p2);
            }
        }
    }

    return QPair<BetterPolygon, BetterPolygon>(*this, BetterPolygon());
}

QPair<BetterPolygon, BetterPolygon> BetterPolygon::splitPolygon(QPair<int, int> chord) const
{
    int i = chord.first, j = chord.second;

    if (!chordIsClear(i, j))
        return QPair<BetterPolygon, BetterPolygon>(*this, BetterPolygon());

    QVector<QPointF> p1, p2;

    int progress = i;
    while (true) {
        if (progress == j) {
            p1.append(mPolygon[progress]);
            break;
        }

        p1.append(mPolygon[progress]);
        progress = (progress + 1) % mPolygon.size();
    }
    while (true) {
        if (progress == i) {
            p2.append(mPolygon[progress]);
            break;
        }

        p2.append(mPolygon[progress]);
        progress = (progress + 1) % mPolygon.size();
    }

    return QPair<BetterPolygon, BetterPolygon>(p1, p2);
}

QList<Triplet<QPointF, QPointF, QPointF>> BetterPolygon::triangulate() const
{
    Q_ASSERT(mPolygon.size() >= 3);

    if (mPolygon.size() == 3) {
        if (Geometry::orientation(mPolygon[0], mPolygon[1], mPolygon[2]) == 2)
            return { {mPolygon[0], mPolygon[1], mPolygon[2]} };
        else
            return { {mPolygon[0], mPolygon[2], mPolygon[1]} };
    }

    QPair<BetterPolygon, BetterPolygon> split = splitPolygon();
    QList<Triplet<QPointF, QPointF, QPointF>> trigs = split.first.triangulate();
    trigs.append(split.second.triangulate());

    return trigs;
}

QVector<BetterPolygon> parseQPolygonF(QPolygonF base)
{
    if (base.isEmpty()) return {};

    if (base.isClosed()) base.removeLast();
    if (BetterPolygon(base).isValid()) return { BetterPolygon(base) };

    //The following is very specific to how QPolygonF seems to output TODO add details about how this works.

    QVector<BetterPolygon> polygons;

    QPointF startPoint = base.first();
    QPolygonF currentPoly;
    currentPoly.append(startPoint);

    //parse the first polygon
    auto i = base.begin() + 1;
    while (*i != startPoint) {
        currentPoly.append(*i);
        ++i;
    }

    polygons.append(currentPoly);
    currentPoly = QPolygonF();
    if (++i == base.end()) return polygons;
    currentPoly.append(startPoint = *(i++));
    while(i != base.end()) {
        if (*i == startPoint) {
            polygons.append(currentPoly);
            currentPoly = QPolygonF();
            if (++i == base.end()) return polygons;
            if (++i == base.end()) return polygons;
            startPoint = *i;
        }

        currentPoly.append(*(i++));
    }

    return polygons;
}

QVector<BetterPolygon> BetterPolygon::subtract(const BetterPolygon &other) const
{
    bool firstPointContained = mPolygon.containsPoint(other.mPolygon.first(), Qt::OddEvenFill);

    if (!overlaps(other)) {
        if (firstPointContained) {
            //Special case, polygon to subtract is contained within this
            auto findConnection = [this, other](int &i, int &j) {
                while(true) {
                    bool shouldBreak = false;
                    while(true) {
                        if (connectionIsClear(other, i, j)) {
                            shouldBreak = true;
                            break;
                        }

                        j = (j + 1) % other.mPolygon.size();
                    }
                    if (shouldBreak) break;

                    i = (i + 1) % mPolygon.size();
                }
            };

            QPair<int, int> cA, cB;
            findConnection(cA.first = 0,
                           cA.second = 0);
            findConnection(cB.first = (cA.first + 1) % mPolygon.size(),
                           cB.second = (cA.second + 1) % other.mPolygon.size());

            //Construct the two new polygons.
            QPolygonF polyA;
            polyA.append(mPolygon[cA.first]);
            polyA.append(other.mPolygon[cA.second]);

            int prog = (cA.second + 1) % other.mPolygon.size();
            while (prog != cB.second) {
                polyA.append(other.mPolygon[prog]);
                prog = (prog + 1) % other.mPolygon.size();
            }

            polyA.append(other.mPolygon[cB.second]);
            polyA.append(mPolygon[cB.first]);

            prog = (cB.first - 1 + mPolygon.size()) % mPolygon.size();
            while (prog != cA.first) {
                polyA.append(mPolygon[prog]);
                prog = (prog - 1 + mPolygon.size()) % mPolygon.size();
            }

            QPolygonF polyB;
            polyB.append(mPolygon[cA.first]);
            polyB.append(other.mPolygon[cA.second]);

            prog = (cA.second - 1 + other.mPolygon.size()) % other.mPolygon.size();
            while (prog != cB.second) {
                polyB.append(other.mPolygon[prog]);
                prog = (prog - 1 + other.mPolygon.size()) % other.mPolygon.size();
            }

            polyB.append(other.mPolygon[cB.second]);
            polyB.append(mPolygon[cB.first]);

            prog = (cB.first + 1) % mPolygon.size();
            while (prog != cA.first) {
                polyB.append(mPolygon[prog]);
                prog = (prog + 1) % mPolygon.size();
            }

            return { polyA, polyB };

        } else {
            if (other.mPolygon.containsPoint(mPolygon.first(), Qt::OddEvenFill))
                return {};
            else
                return { *this };
        }
    }

    //The base subraction of the two polygons. Output might not be
    //valid by the rules of BetterPolygon
    QPolygonF base = mPolygon.subtracted(other.mPolygon);

    return parseQPolygonF(base);
}

QVector<BetterPolygon> BetterPolygon::intersect(const BetterPolygon &other) const
{
    auto sub = subtract(other);

    if (sub.size() == 0) return { *this };
    if (sub.size() == 1) return subtract(sub.first());

    QVector<BetterPolygon> ret  = subtract(sub.first());

    for (auto i = sub.cbegin() + 1; i != sub.cend(); ++i) {
        QVector<BetterPolygon> tmp;

        for (const BetterPolygon &p : ret)
            tmp.append(p.subtract(*i));

        ret.swap(tmp);
    }

    return ret;
}

QVector<BetterPolygon> BetterPolygon::united(const BetterPolygon &other) const
{
    QPolygonF base = mPolygon.united(other.mPolygon);

    return parseQPolygonF(base);
}
