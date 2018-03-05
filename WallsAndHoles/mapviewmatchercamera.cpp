#include "mapviewmatchercamera.h"

MapViewMatcherCamera::MapViewMatcherCamera(MapViewContainer *mapViewContainer, QObject *parent)
    : AbstractMeshViewCamera(parent)
{
    mTransformationMatrix.setToIdentity();
}

QMatrix4x4 MapViewMatcherCamera::getTransformationMatrix() const
{

}

QVector3D MapViewMatcherCamera::getPosition() const
{

}

void MapViewMatcherCamera::mapViewChanged(QRect viewRect)
{

}
