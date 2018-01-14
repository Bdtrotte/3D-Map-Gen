#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QSharedPointer>

#include "renderableobject.h"


class Scene {
    using RenderableObjectP = QSharedPointer<RenderableObject>;

public:
    Scene();

    // Adds an object to the scene.
    void addObject(RenderableObjectP object);

    // Computes the number of vertices in the scene by adding up the number of vertices
    // in each object. This does not consider whether vertices are reused.
    size_t getNumVertices() const;

    const QVector<RenderableObjectP>& getAllObjects() const;

protected:

    QVector<RenderableObjectP> objects;
};

#endif // SCENE_H
