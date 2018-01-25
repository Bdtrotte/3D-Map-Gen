#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QSharedPointer>

#include "renderableobject.h"
#include "abstractdrawableglobject.h"

class Scene {
    using RenderableObjectP = QSharedPointer<RenderableObject>;
    using DrawableObjectP = QSharedPointer<AbstractDrawableGLObject>;

public:
    Scene();

    /**
     * @brief Adds a RenderableObject to the scene.
     * @param object The object to be added.
     */
    void addObject(RenderableObjectP object);

    /**
     * @brief Adds a DrawableObject to the scene.
     * @param object The object to be added.
     */
    void addObject(DrawableObjectP object);

    // Computes the number of vertices in the scene by adding up the number of vertices
    // in each object. This does not consider whether vertices are reused.
    size_t getNumVertices() const;


    const QVector<DrawableObjectP>& getAllDrawables() const;
    const QVector<RenderableObjectP>& getAllObjects() const;

protected:

    QVector<DrawableObjectP> drawables;
    QVector<RenderableObjectP> objects;
};

#endif // SCENE_H
