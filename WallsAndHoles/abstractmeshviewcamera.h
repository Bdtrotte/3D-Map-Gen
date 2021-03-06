#ifndef MESHVIEWCAMERA_H
#define MESHVIEWCAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>


#include "abstracttool.h"

/**
 * @brief The AbstractMeshViewCamera class
 * Abstract class that will determine how the camera moves based on the mouse.
 */
class AbstractMeshViewCamera : public AbstractTool {
    Q_OBJECT

public:
    virtual ~AbstractMeshViewCamera() {}

    /**
     * @brief getTransformationMatrix
     * @return the transform matrix that should be premultiplied to the vertex world-space positions
     */
    virtual QMatrix4x4 getTransformationMatrix() const = 0;

    /**
     * @brief Gets the world-space camera position.
     * @return The world-space camera position.
     */
    virtual QVector3D getPosition() const = 0;


    virtual void activate() override {}
    virtual void deactivate() override {}

    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void wheelEvent(QWheelEvent *) override {}

signals:
    /**
     * @brief This signal should be emitted when the camera is moved.
     */
    void changed();
};

#endif // MESHVIEWCAMERA_H
