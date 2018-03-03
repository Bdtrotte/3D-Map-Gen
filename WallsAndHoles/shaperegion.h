#ifndef SHAPEREGION_H
#define SHAPEREGION_H

#include <QRegion>

namespace ShapeRegion {

void findBounds(const QPoint &start, const QPoint &end, QPoint &topLeft, QPoint &bottomRight);

QRegion rect(QPoint start, QPoint end);
QRegion rectOutline(QPoint start, QPoint end);

QRegion ellipseOutline(QPoint start, QPoint end);

QRegion line(QPoint start, QPoint end);

}

#endif // SHAPEREGION_H
