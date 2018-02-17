
// Used in cleanMaps()
#include <QSet>
#include <QMutableMapIterator>

// For std::reference_wrapper
#include <functional>

#include "simpletexturedrenderer.h"

SimpleTexturedRenderer::SimpleTexturedRenderer(SharedSimpleTexturedScene scene)
    : mScene(scene)
{

}

SimpleTexturedRenderer::~SimpleTexturedRenderer()
{

}


void SimpleTexturedRenderer::objectAdded(const SimpleTexturedObject &obj)
{
    // obj must be wrapped in a reference_wrapper because references are not
    // otherwise copy-constructible
    callFunctionOnOpenGL(&SimpleTexturedRenderer::createObjectBuffers, this, std::reference_wrapper<const SimpleTexturedObject>(obj));
}


void SimpleTexturedRenderer::objectRemoved(const SimpleTexturedObject &obj)
{
    QMutexLocker locker(&mGLDataMutex);

    // It is safe to use QMap::remove() even if the key might not be in the map.
    // Destroying buffers doesn't seem to require a valid OpenGL context.
    mVAOs.remove(&obj);
    mObjectVertexPositions.remove(&obj);
    mObjectVertexNormals.remove(&obj);
    mObjectVertexMaterials.remove(&obj);
    mObjectVertexTexCoords.remove(&obj);
    mNumVertices.remove(&obj);

    // If the object's image has an associated texture, remove the object
    // from the texture's set.
    if (mImagesToTextures.contains(&obj.getImage())) {
        QSharedPointer<QOpenGLTexture> texturePtr = mImagesToTextures[&obj.getImage()];

        auto &textureUsageSet = mTexturesToObjects[texturePtr.data()];

        textureUsageSet.remove(&obj);

        // If the texture has no users, unload it.
        if (textureUsageSet.size() <= 0) {
            // It is important to unlock and relock the mutex here, just in case
            // callFunctionOnOpenGL() happens in this thread.
            locker.unlock();
            callFunctionOnOpenGL(&SimpleTexturedRenderer::removeImageTexture, this, &obj.getImage());
            locker.relock();
        }
    }
}


void SimpleTexturedRenderer::clearAll()
{
    QMutexLocker locker(&mGLDataMutex);

    // Destroying buffers does not require a valid OpenGL context. However,
    // the same is not true for textures.
    mVAOs.clear();
    mObjectVertexPositions.clear();
    mObjectVertexNormals.clear();
    mObjectVertexMaterials.clear();
    mObjectVertexTexCoords.clear();
    mNumVertices.clear();

    // TODO: Is it possible that new images might be added before this happens?
    callFunctionOnOpenGL(&SimpleTexturedRenderer::clearAllTextures, this);
}

void SimpleTexturedRenderer::cleanUp()
{
    QMutexLocker locker(&mGLDataMutex);

    mVAOs.clear();
    mObjectVertexPositions.clear();
    mObjectVertexNormals.clear();
    mObjectVertexMaterials.clear();
    mObjectVertexTexCoords.clear();
    mNumVertices.clear();

    mShaderProgram.destroy();

    // Unlock the data mutex because the clearAllTextures() function uses it.
    locker.unlock();

    clearAllTextures();
}


void SimpleTexturedRenderer::paint(QMatrix4x4 mvpMatrix, QVector3D camPos)
{
    QMutexLocker locker(&mGLDataMutex);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set program.
    mShaderProgram.bind();

    // Set the matrix.
    mShaderProgram.setUniformMVP(mvpMatrix);

    // Set the lighting uniforms.
    mShaderProgram.setUniformPointToLight(QVector3D(1, 1, 0).normalized());
    mShaderProgram.setUniformCameraPosition(camPos);
    mShaderProgram.setUniformAmbientColor(QVector3D(0.1, 0.1, 0.2));
    mShaderProgram.setUniformSourceDiffuseColor(QVector3D(0.3, 0.3, 0.2));
    mShaderProgram.setUniformSourceSpecularColor(QVector3D(0.3, 0.3, 0.2));

    // Draw objects by texture group.
    foreach (const QImage *img, mImagesToTextures.keys()) {
        QSharedPointer<QOpenGLTexture> texture = mImagesToTextures[img];

        const auto &objectsUsingTexture = mTexturesToObjects[texture.data()];

        // Bind texture.
        mShaderProgram.bindUniformTexture(*texture);


        // Draw all objects that are using this texture.
        foreach (const SimpleTexturedObject *obj, objectsUsingTexture) {
            auto vao = mVAOs[obj];
            int numVertices = mNumVertices[obj];

            vao->bind();

            mShaderProgram.enableArrays();
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
            mShaderProgram.disableArrays();

            vao->release();
        }


        // Release texture.
        mShaderProgram.releaseUniformTexture(*texture);
    }

    // Unset program.
    mShaderProgram.release();


    glDisable(GL_DEPTH_TEST);


    locker.unlock();

    // For good measure! This will print errors to qDebug() if there are any.
    checkGLErrors();
}


void SimpleTexturedRenderer::initializeRenderer()
{
    // This will create a new shader program and destroy the old one (if any).
    mShaderProgram.create();

    // TODO: This needs to happen on the original GL context.
//    clearAllTextures();

    // This will remake the buffers for all objects in the scene.
    createSceneBuffers();

}


void SimpleTexturedRenderer::createSceneBuffers()
{
    for (const SimpleTexturedObject &obj : *mScene)
        createObjectBuffers(obj);
}

void SimpleTexturedRenderer::createObjectBuffers(const SimpleTexturedObject &obj)
{

    QVector<GLfloat> vertices;
    QVector<GLfloat> normals;
    QVector<GLfloat> materials;
    QVector<GLfloat> texCoords;

    // Aggregate information into arrays that will be uploaded to OpenGL buffers.
    // Triangles will be unpacked.

    const auto &objVertices = obj.getVertices();
    const auto &objFaceNormals = obj.getFaceNormals();
    const auto &objTriangles = obj.getTriangles();
    const auto &objAmbient = obj.getVertexAmbient();
    const auto &objDiffuse = obj.getVertexDiffuse();
    const auto &objSpecular = obj.getVertexSpecular();
    const auto &objShininess = obj.getVertexShininess();
    const auto &objFaceTexCoords = obj.getFaceTexCoords();

    for (int triangleIdx = 0; triangleIdx < obj.getNumTriangles(); ++triangleIdx) {

        auto triangle = objTriangles[triangleIdx];

        int vIdx1 = triangle.getFirst();
        int vIdx2 = triangle.getSecond();
        int vIdx3 = triangle.getThird();

        QVector3D v1 = objVertices[vIdx1];
        QVector3D v2 = objVertices[vIdx2];
        QVector3D v3 = objVertices[vIdx3];

        auto triangleTexCoords = objFaceTexCoords[triangleIdx];
        QVector2D t1 = triangleTexCoords.getFirst();
        QVector2D t2 = triangleTexCoords.getSecond();
        QVector2D t3 = triangleTexCoords.getThird();

        QVector3D normal = objFaceNormals[triangleIdx];

        for (QVector3D v : {v1, v2, v3}) {
            vertices.append(v[0]);
            vertices.append(v[1]);
            vertices.append(v[2]);
        }

        for (QVector2D t : {t1, t2, t3}) {
            texCoords.append(t[0]);
            texCoords.append(t[1]);
        }

        // Same normal for each vertex in one face.
        for (int i = 0; i < 3; ++i)
            normals.append({normal[0], normal[1], normal[2]});




        for (int vIdx : {vIdx1, vIdx2, vIdx3}) {
            materials.append(objAmbient[vIdx]);
            materials.append(objDiffuse[vIdx]);
            materials.append(objSpecular[vIdx]);
            materials.append(objShininess[vIdx]);
        }
    }



    auto vertexPositions = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    vertexPositions->create();
    vertexPositions->bind();
    vertexPositions->setUsagePattern(QOpenGLBuffer::DynamicDraw); // may be updated in real-time in the future
    vertexPositions->allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    vertexPositions->release();

    auto vertexNormals = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    vertexNormals->create();
    vertexNormals->bind();
    vertexNormals->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexNormals->allocate(normals.data(), normals.size() * sizeof(GLfloat));
    vertexNormals->release();

    auto vertexMaterials = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    vertexMaterials->create();
    vertexMaterials->bind();
    vertexMaterials->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexMaterials->allocate(materials.data(), materials.size() * sizeof(GLfloat));
    vertexMaterials->release();

    auto vertexTexCoords = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    vertexTexCoords->create();
    vertexTexCoords->bind();
    vertexNormals->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexNormals->allocate(texCoords.data(), texCoords.size() * sizeof(GLfloat));
    vertexNormals->release();


    auto vao = QSharedPointer<QOpenGLVertexArrayObject>::create(nullptr);
    vao->create();
    vao->bind();

    vertexPositions->bind();
    mShaderProgram.setAttrPositionBuffer();
    vertexPositions->release();

    vertexNormals->bind();
    mShaderProgram.setAttrNormalBuffer();
    vertexNormals->release();

    vertexTexCoords->bind();
    mShaderProgram.setAttrTexCoordsBuffer();
    vertexTexCoords->release();

    vertexMaterials->bind();
    mShaderProgram.setAttrReflAmbientBuffer(0 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflDiffuseBuffer(1 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflSpecularBuffer(2 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrShininessBuffer(3 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    vertexMaterials->release();


    vao->release();


    QMutexLocker locker(&mGLDataMutex);

    mVAOs[&obj] = vao;
    mObjectVertexPositions[&obj] = vertexPositions;
    mObjectVertexNormals[&obj] = vertexNormals;
    mObjectVertexTexCoords[&obj] = vertexTexCoords;
    mObjectVertexMaterials[&obj] = vertexMaterials;
    mNumVertices[&obj] = vertices.size() / 3;


    // If the image associated to the object has not yet been created, create it.
    if (!mImagesToTextures.contains(&obj.getImage())) {
        QSharedPointer<QOpenGLTexture> newTexture = QSharedPointer<QOpenGLTexture>::create(obj.getImage().mirrored());
        newTexture->setWrapMode(QOpenGLTexture::Repeat);

        mImagesToTextures.insert(&obj.getImage(), newTexture);
        mTexturesToObjects.insert(newTexture.data(), QSet<const SimpleTexturedObject *>());
    }

    // Record that the object uses the texture.
    mTexturesToObjects[mImagesToTextures[&obj.getImage()].data()].insert(&obj);
}


#define REMOVE_INVALID_KEYS(validKeys, keyItr) while (keyItr.hasNext()) if(!validKeys.contains(keyItr.next().key())) keyItr.remove();

void SimpleTexturedRenderer::cleanMaps()
{
    QSet<const SimpleTexturedObject *> validKeys;

    for (const SimpleTexturedObject &obj : *mScene)
        validKeys.insert(&obj);


    QMutexLocker locker(&mGLDataMutex);

    auto keyItr1 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mVAOs)::mapped_type>(mVAOs);
    auto keyItr2 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexPositions)::mapped_type>(mObjectVertexPositions);
    auto keyItr3 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexNormals)::mapped_type>(mObjectVertexNormals);
    auto keyItr4 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexMaterials)::mapped_type>(mObjectVertexMaterials);
    auto keyItr5 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mNumVertices)::mapped_type>(mNumVertices);
    auto keyItr6 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexTexCoords)::mapped_type>(mObjectVertexTexCoords);

    // For each map, go through its keys and remove invalid ones.
    REMOVE_INVALID_KEYS(validKeys, keyItr1);
    REMOVE_INVALID_KEYS(validKeys, keyItr2);
    REMOVE_INVALID_KEYS(validKeys, keyItr3);
    REMOVE_INVALID_KEYS(validKeys, keyItr4);
    REMOVE_INVALID_KEYS(validKeys, keyItr5);
    REMOVE_INVALID_KEYS(validKeys, keyItr6);


    // Also clean up mImagesToTextures and mTexturesToObjects.
    foreach (const SimpleTexturedObject *obj, validKeys) {
        const QImage *img = &obj->getImage();

        if (mImagesToTextures.contains(img)) {
            QSharedPointer<QOpenGLTexture> texture = mImagesToTextures[img];

            mTexturesToObjects[texture.data()].remove(obj);

            // If now no object is using this texture, destroy the texture.
            if (mTexturesToObjects[texture.data()].isEmpty())
                removeImageTexture(img);
        }
    }
}

#undef REMOVE_INVALID_KEYS


void SimpleTexturedRenderer::clearAllTextures()
{
    QMutexLocker locker(&mGLDataMutex);

    mTexturesToObjects.clear();

    foreach (QSharedPointer<QOpenGLTexture> texture, mImagesToTextures.values())
        texture->destroy();

    mImagesToTextures.clear();
}


void SimpleTexturedRenderer::removeImageTexture(const QImage *image)
{
    QMutexLocker locker(&mGLDataMutex);

    QSharedPointer<QOpenGLTexture> texture = mImagesToTextures[image];
    texture->destroy();

    mTexturesToObjects.remove(texture.data());
    mImagesToTextures.remove(image);
}