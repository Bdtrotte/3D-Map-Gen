#include "meshviewcameralikeblender.h"

MeshViewCameraLikeBlender::MeshViewCameraLikeBlender()
    : mCenterOfRotation(0, 0, 0),
      mDistanceFromCenter(5),
      mRotationPitch(0),
      mRotationYaw(0)
{

}

QMatrix4x4 MeshViewCameraLikeBlender::getTransformationMatrix() const {
    QMatrix4x4 trans;


    trans.translate(0, 0, -mDistanceFromCenter);

    trans.rotate(mRotationPitch, 1, 0, 0);
    trans.rotate(mRotationYaw, 0, 1, 0);

    trans.translate(-mCenterOfRotation);

    return trans;
}


void MeshViewCameraLikeBlender::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mRotationPitchStart = mRotationPitch;
        mRotationYawStart = mRotationYaw;
        mCursorStart = event->pos();
        mStartTime = event->timestamp();
        event->accept();
    }
}


void MeshViewCameraLikeBlender::mouseMoveEvent(QMouseEvent *event) {
    if (event->timestamp() > mStartTime) {
        QPoint deltaPos = event->pos() - mCursorStart;

        mRotationYaw = mRotationYawStart + deltaPos.x();
        mRotationPitch = mRotationPitchStart + deltaPos.y();

        event->accept();
    }
}
