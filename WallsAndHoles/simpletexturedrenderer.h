#ifndef SIMPLETEXTUREDRENDERER_H
#define SIMPLETEXTUREDRENDERER_H

#include <QSharedPointer>
#include <QMatrix4x4>
#include <QMap>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "abstractrenderer.h"

#include "simpletexturedshader.h"
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
     * Locks the mGLDataMutex.
     *
     * @param obj   The object that is already removed or will be removed.
     */
    void objectRemoved(const SimpleTexturedObject &obj);


    /**
     * @brief Clears all OpenGL data, not assuming that a context is current. Some data
     * may not be cleared immediately. Locks the mGLDataMutex.
     */
    void clearAll();


    /**
     * @brief Cleans up all OpenGL data. This assumes that the correct context is current,
     * and all data is cleaned immediately. Locks the mGLDataMutex.
     */
    void cleanUp() override;

protected:

    void initializeRenderer() override;


private:

    /**
     * @brief Creates vertex arrays and buffers for each object in current scene.
     * It is assumed that an OpenGL context is bound. Calls createObjectBuffers(), which
     * locks the mGLDataMutex.
     */
    void createSceneBuffers();


    /**
     * @brief Creates vertex arrays and buffers for the given object (assumed to be in the scene).
     * It is assumed that an OpenGL context is bound. Locks the mGLDataMutex.
     */
    void createObjectBuffers(const SimpleTexturedObject &obj);

    /**
     * @brief Goes through the QMaps whose keys are SimpleTexturedObjects and gets rid of
     * objects that no longer exist. Assumes the correct OpenGL context is bound. Locks the mGLDataMutex.
     */
    void cleanMaps();

    /**
     * @brief Unloads all textures. Assumes the correct OpenGL context is bound. Locks the mGLDataMutex.
     */
    void clearAllTextures();

    /**
     * @brief Unloads the texture associated with the image. This must be run on the thread
     * with a valid OpenGL context. Locks the mGLDataMutex.
     */
    void removeImageTexture(const QImage *image);



    SharedSimpleTexturedScene mScene;

    SimpleTexturedShader mShaderProgram;


    /// QMutex that should be used when modifying or using any of the below variables.
    QMutex mGLDataMutex;


    // VAO and buffers for each object.
    // TODO These should be adapted to a textured shader.
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLVertexArrayObject>> mVAOs;
    QMap<const SimpleTexturedObject *, int> mNumVertices;

    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexPositions;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexNormals;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexMaterials;
    QMap<const SimpleTexturedObject *, QSharedPointer<QOpenGLBuffer>> mObjectVertexTexCoords;



    /// A map from existing images to their associated created textures. This is to avoid
    /// allocating a whole texture per object when a new object is added.
    ///
    /// This variable also owns the textures.
    QMap<const QImage *, QSharedPointer<QOpenGLTexture>> mImagesToTextures;


    /// A map from textures to the objects that use them.
    QMap<const QOpenGLTexture *, QSet<const SimpleTexturedObject *>> mTexturesToObjects;

};

#endif // SIMPLETEXTUREDRENDERER_H
