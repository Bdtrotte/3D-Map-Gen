#ifndef MESHVIEWCAMERA_H
#define MESHVIEWCAMERA_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>


/**
 * @brief The AbstractMeshViewCamera class
 * Abstract class that will determine how the camera moves based on the mouse.
 */
class AbstractMeshViewCamera {
public:
    virtual ~AbstractMeshViewCamera() {}

    /**
     * @brief getTransformationMatrix
     * @return the transform matrix that should be premultiplied to the vertex world-space positions
     */
    virtual QMatrix4x4 getTransformationMatrix() const = 0;


    virtual void mousePressEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}
    virtual void wheelEvent(QWheelEvent *) {}
};

#endif // MESHVIEWCAMERA_H
