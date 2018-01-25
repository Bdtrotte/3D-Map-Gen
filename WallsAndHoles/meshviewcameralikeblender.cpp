#include <QApplication>
#include <QtMath>

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
        mCenterStart = mCenterOfRotation;
        mRotating = (QApplication::keyboardModifiers() & Qt::ShiftModifier) == 0;
        event->accept();
    }
}


void MeshViewCameraLikeBlender::mouseMoveEvent(QMouseEvent *event) {
    if (event->timestamp() > mStartTime) {
        QPoint deltaPos = event->pos() - mCursorStart;
        if (mRotating) {
            mRotationYaw = mRotationYawStart + deltaPos.x();
            mRotationPitch = mRotationPitchStart + deltaPos.y();
        } else {
            QQuaternion rotation = QQuaternion::fromAxisAndAngle(1, 0, 0, -mRotationPitch);
            rotation = QQuaternion::fromAxisAndAngle(0, 1, 0, -mRotationYaw) * rotation;

            QVector3D up = rotation.rotatedVector(QVector3D(0, 1, 0));
            QVector3D right = rotation.rotatedVector(QVector3D(-1, 0, 0));

            mCenterOfRotation = mCenterStart + 0.01 * up * deltaPos.y() + 0.01 * right * deltaPos.x();
        }

        event->accept();
    }
}


void MeshViewCameraLikeBlender::wheelEvent(QWheelEvent *event) {
    mDistanceFromCenter *= pow(1.02, event->delta());
    event->accept();
}
