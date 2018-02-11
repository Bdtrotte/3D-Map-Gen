#ifndef SIMPLETEXTUREDRENDERER_H
#define SIMPLETEXTUREDRENDERER_H

#include <QSharedPointer>
#include <QMatrix4x4>
#include <QMap>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "abstractrenderer.h"

#include "shaderprogramonelight.h"
#include "simpletexturedscene.h"
#include "simpletexturedobject.h"


/**
 * @brief The SimpleTexturedRenderer class renders a SimpleTexturedScene.
 */
class SimpleTexturedRenderer : public AbstractRenderer
{
    Q_OBJECT

public:
    SimpleTexturedRenderer(SharedSimpleTexturedScene scene);

    virtual ~SimpleTexturedRenderer();

    void paint(QMatrix4x4 mvpMatrix, QVector3D camPos) override;


public slots:
    /**
     * @brief This slot should be called whenever the underlying scene has a new object.
     * @param obj   The new object.
     */
    void objectAdded(const SimpleTexturedObject &obj);


    /**
     * @brief This slot should be called whenever an object is removed from the underlying scene.
     * @param obj   The object that is already removed or will be removed.
     */
    void objectRemoved(const SimpleTexturedObject &obj);


    /**
     * @brief Clears all OpenGL data.
     */
    void clearAll();

protected:

    void initializeRenderer() override;


private:

    /**
     * @brief Creates vertex arrays and buffers for each object in current scene.
     * It is assumed that an OpenGL context is bound.
     */
    void createSceneBuffers();


    /**
     * @brief Creates vertex arrays and buffers for the given object (assumed to be in the scene).
     * It is assumed that an OpenGL context is bound.
     */
    void createObjectBuffers(const SimpleTexturedObject &obj);

    /**
     * @brief Goes through the QMaps whose keys are SimpleTexturedObjects and gets rid of
     * objects that no longer exist.
     */
    void cleanMaps();


    SharedSimpleTexturedScene mScene;

    // TODO This should be a textured shader.
    ShaderProgramOneLight mShaderProgram;

    // VAO and buffers for each object.
    // TODO These should be adapted to a textured shader.
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLVertexArrayObject>> mVAOs;
    QMap<const SimpleTexturedObject *, int> mNumVertices;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexPositions;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexNormals;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexMaterials;
};

#endif // SIMPLETEXTUREDRENDERER_H
