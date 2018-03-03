#ifndef ARRAY2DTOOLS_H
#define ARRAY2DTOOLS_H

#include <QSize>
#include <QSet>
#include <QPoint>

// QPoints are not hashable in Qt by default.
inline uint qHash (const QPoint & key)
{
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

/**
 * @brief Returns true if the point is in bounds, false otherwise.
 */
bool isPointInBounds(int x, int y, int width, int height)
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool isPointInBounds(int x, int y, QSize size)
{
    return isPointInBounds(x, y, size.width(), size.height());
}


bool isPointInBounds(QPoint pt, QSize size)
{
    return isPointInBounds(pt.x(), pt.y(), size.width(), size.height());
}


/**
 * @brief Returns the coordinates of all (up to 8) in-bounds neighbors of the point.
 */
QSet<QPoint> getValidNeighbors(int x, int y, int width, int height)
{
    QSet<QPoint> neighbors;

    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            if (!(x == 1 && y == 1) && isPointInBounds(x+dx, y+dy, width, height))
                neighbors.insert({x+dx, y+dy});

    return neighbors;
}

QSet<QPoint> getValidNeighbors(int x, int y, QSize size)
{
    return getValidNeighbors(x, y, size.width(), size.height());
}

QSet<QPoint> getValidNeighbors(QPoint pt, QSize size)
{
    return getValidNeighbors(pt.x(), pt.y(), size.width(), size.height());
}


#endif // ARRAY2DTOOLS_H
