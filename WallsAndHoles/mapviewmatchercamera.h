#ifndef MAPVIEWMATCHERCAMERA_H
#define MAPVIEWMATCHERCAMERA_H

#include "abstractmeshviewcamera.h"
#include "mapviewcontainer.h"

class MapViewMatcherCamera : public AbstractMeshViewCamera
{
    Q_OBJECT

public:
    MapViewMatcherCamera(MapViewContainer *mapViewContainer, QObject *parent = nullptr);

    QMatrix4x4 getTransformationMatrix() const override;
    QVector3D getPosition() const override;

    void mousePressEvent(QMouseEvent *) override   { emit receivedInteraction(); }
    void mouseReleaseEvent(QMouseEvent *) override { emit receivedInteraction(); }
    void mouseMoveEvent(QMouseEvent *) override    { emit receivedInteraction(); }
    void wheelEvent(QWheelEvent *) override        { emit receivedInteraction(); }

signals:
    /**
     * @brief receivedInteraction
     *
     * Emitted whenever the meshView gets input. Can be used to choose to
     * change camera.
     */
    void receivedInteraction();

private slots:
    /**
     * @brief mapViewChanged
     *
     * gets the viewRect when the mapview changes. The coords
     * of the rect are in tile coords.
     *
     * @param topLeft
     * @param botRight
     */
    void mapViewChanged(QRect viewRect);

private:
    QMatrix4x4 mTransformationMatrix;
    QVector3D mPosition;
};

#endif // MAPVIEWMATCHERCAMERA_H
