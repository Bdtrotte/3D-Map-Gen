#include <QApplication>
#include <QtMath>

#include "meshviewcameralikeblender.h"

MeshViewCameraLikeBlender::MeshViewCameraLikeBlender()
    : mCenterOfRotation(0, 0, 0),
      mDistanceFromCenter(45),
      mRotationPitch(0),
      mRotationYaw(0)
{

}

QMatrix4x4 MeshViewCameraLikeBlender::getTransformationMatrix() const {
    QMatrix4x4 worldToCamera;


    worldToCamera.translate(0, 0, -mDistanceFromCenter);

    worldToCamera.rotate(mRotationPitch, 1, 0, 0);
    worldToCamera.rotate(mRotationYaw, 0, 1, 0);

    worldToCamera.translate(-mCenterOfRotation);

    return worldToCamera;
}


QVector3D MeshViewCameraLikeBlender::getPosition() const {

    // This is just the inverse of the worldToCamera matrix in getTransformationMatrix().
    // Instead of computing the matrix and then computing its inverse, we just apply
    // operations in reverse order.
    QMatrix4x4 cameraToWorld;

    cameraToWorld.translate(mCenterOfRotation);

    cameraToWorld.rotate(-mRotationYaw, 0, 1, 0);
    cameraToWorld.rotate(-mRotationPitch, 1, 0, 0);

    cameraToWorld.translate(0, 0, mDistanceFromCenter);

    return QVector3D(cameraToWorld * QVector4D(0, 0, 0, 1));
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

            mCenterOfRotation = mCenterStart + 0.005 * mDistanceFromCenter * (up * deltaPos.y() + right * deltaPos.x());
        }

        event->accept();
    }
}


void MeshViewCameraLikeBlender::wheelEvent(QWheelEvent *event) {
    mDistanceFromCenter *= pow(1.02, event->delta());
    event->accept();
}
