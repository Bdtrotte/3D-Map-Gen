#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPointF>
#include <QVector3D>
#include <QtMath>

namespace Geometry {

//0 - a, b, and c are colinear
//1 - Clockwise
//2 - Counter Clockwise
inline int orientation(QPointF a, QPointF b, QPointF c)
{
    QVector3D A(b.x() - a.x(), 0, b.y() - a.y());
    QVector3D B(c.x() - a.x(), 0, c.y() - a.y());

    QVector3D AxB = QVector3D::crossProduct(A, B);

    if (qAbs(AxB.y()) <= 0.01) return 0;
    if (AxB.y() > 0)  return 2;
    else              return 1;
}

}

#endif // GEOMETRY_H
