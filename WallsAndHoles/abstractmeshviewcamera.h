#ifndef MESHVIEWCAMERA_H
#define MESHVIEWCAMERA_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>

#include "abstracttool.h"

/**
 * @brief The AbstractMeshViewCamera class
 * Abstract class that will determine how the camera moves based on the mouse.
 */
class AbstractMeshViewCamera : public AbstractTool {
public:
    virtual ~AbstractMeshViewCamera() {}

    /**
     * @brief getTransformationMatrix
     * @return the transform matrix that should be premultiplied to the vertex world-space positions
     */
    virtual QMatrix4x4 getTransformationMatrix() const = 0;


    virtual void activate() override {}
    virtual void deactivate() override {}

    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void wheelEvent(QWheelEvent *) override {}
};

#endif // MESHVIEWCAMERA_H
