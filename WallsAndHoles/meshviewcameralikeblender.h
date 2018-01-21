#ifndef MESHVIEWCAMERALIKEBLENDER_H
#define MESHVIEWCAMERALIKEBLENDER_H

#include <QVector3D>

#include "meshviewcamera.h"

class MeshViewCameraLikeBlender : public MeshViewCamera {
public:
    MeshViewCameraLikeBlender();
    ~MeshViewCameraLikeBlender() {}

    QMatrix4x4 getTransformationMatrix() const override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    QVector3D mCenterOfRotation;
    float mDistanceFromCenter;
    float mRotationPitch;
    float mRotationYaw;


    // The rotation and cursor position when the mouse was first pressed down for dragging.
    float mRotationPitchStart;
    float mRotationYawStart;
    QPoint mCursorStart;
    ulong mStartTime; // the timestamp of the most recent mouse-press event
};

#endif // MESHVIEWCAMERALIKEBLENDER_H
