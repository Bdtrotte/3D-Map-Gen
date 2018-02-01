#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QSharedPointer>
#include <QObject>

#include "renderableobject.h"
#include "abstractdrawableglobject.h"

class Scene : public QObject {

    Q_OBJECT

public:

    using RenderableObjectP = QSharedPointer<RenderableObject>;
    using DrawableObjectP = QSharedPointer<AbstractDrawableGLObject>;

    Scene(QObject *parent = nullptr);
    virtual ~Scene() {}

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

    /**
     * @brief Calls initialzeGL() on all drawables.
     *
     * NOTE: For now, this does not need to be a slot. Slot functionality can be added as needed.
     */
    void initializeGL();


signals:
    /**
     * @brief This signal is emitted whenever an object is added to the scene.
     * @param object The object that was added.
     */
    void objectAdded(RenderableObjectP object);


protected:

    QVector<DrawableObjectP> drawables;
    QVector<RenderableObjectP> objects;
};

#endif // SCENE_H
