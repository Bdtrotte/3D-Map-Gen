#include "mapviewmatchercamera.h"

#include <QDebug>

MapViewMatcherCamera::MapViewMatcherCamera(MapView *mapView)
    : AbstractMeshViewCamera()
{
    mTransformationMatrix.setToIdentity();

    connect(mapView, &MapView::mapViewChanged,
            this, &MapViewMatcherCamera::mapViewChanged);
}

QMatrix4x4 MapViewMatcherCamera::getTransformationMatrix() const
{
    return mTransformationMatrix;
}

QVector3D MapViewMatcherCamera::getPosition() const
{
    return mPosition;
}

void MapViewMatcherCamera::mapViewChanged(QRectF viewRect)
{
    QPointF center = QPointF(viewRect.width(), viewRect.height())/2 + viewRect.topLeft();

    // TODO the height is based off the fov being 90 degrees, which is hard
    //      coded somewhere. If fov changes, actual formula is height / (tan(fov/2) * 2).
    mPosition = QVector3D(center.x(), viewRect.height()/2, center.y());

    QMatrix4x4 translate;
    QMatrix4x4 rotate;

    translate.translate(-mPosition);
    rotate.rotate(90, 1, 0, 0);

    mTransformationMatrix = rotate * translate;

    changed();
}
