#ifndef MESHVIEWCAMERA_H
#define MESHVIEWCAMERA_H

#include <QMatrix4x4>
#include <QMouseEvent>


/**
 * @brief The MeshViewCamera class
 * Abstract class that will determine how the camera moves based on the mouse.
 */
class MeshViewCamera {
public:
    virtual ~MeshViewCamera() {}

    /**
     * @brief getTransformationMatrix
     * @return the transform matrix that should be premultiplied to the vertex world-space positions
     */
    virtual QMatrix4x4 getTransformationMatrix() const = 0;


    virtual void mousePressEvent(QMouseEvent *) {}
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}
};

#endif // MESHVIEWCAMERA_H