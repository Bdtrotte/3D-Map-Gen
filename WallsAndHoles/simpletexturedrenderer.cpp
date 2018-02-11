
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
    mVAOs.remove(&obj);
    mObjectVertexPositions.remove(&obj);
    mObjectVertexNormals.remove(&obj);
    mObjectVertexMaterials.remove(&obj);
    mNumVertices.remove(&obj);
}


void SimpleTexturedRenderer::clearAll()
{
    mVAOs.clear();
    mObjectVertexPositions.clear();
    mObjectVertexNormals.clear();
    mObjectVertexMaterials.clear();
    mNumVertices.clear();
}


void SimpleTexturedRenderer::paint(QMatrix4x4 mvpMatrix, QVector3D camPos)
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set program.
    mShaderProgram.bind();

    // Set the matrix.
    mShaderProgram.setUniformMVP(mvpMatrix);

    // Set the lighting uniforms.
    mShaderProgram.setUniformLightPosition(camPos);
    mShaderProgram.setUniformCameraPosition(camPos);
    mShaderProgram.setUniformAmbientColor(QVector3D(0.1, 0.1, 0.2));
    mShaderProgram.setUniformSourceDiffuseColor(QVector3D(0.3, 0.3, 0.2));
    mShaderProgram.setUniformSourceSpecularColor(QVector3D(0.3, 0.3, 0.2));

    // Draw each object
    foreach (auto key, mVAOs.keys()) {

        auto vao = mVAOs[key];
        int numVertices = mNumVertices[key];

        // Set arrays.
        vao->bind();
        mShaderProgram.enableArrays();

        // Draw.
        glDrawArrays(GL_TRIANGLES, 0, numVertices);


        // Unset arrays.
        mShaderProgram.disableArrays();
        vao->release();

    }

    // Unset program.
    mShaderProgram.release();


    glDisable(GL_DEPTH_TEST);
}


void SimpleTexturedRenderer::initializeRenderer()
{
    // This will create a new shader program and destroy the old one (if any).
    mShaderProgram.create();

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

    // Aggregate vertex/normal/material/index information into arrays that will be uploaded to OpenGL buffers.
    // Triangles will be unpacked.

    const auto &objVertices = obj.getVertices();
    const auto &objFaceNormals = obj.getFaceNormals();
    const auto &objTriangles = obj.getTriangles();
    const auto &objAmbient = obj.getVertexAmbient();
    const auto &objDiffuse = obj.getVertexDiffuse();
    const auto &objSpecular = obj.getVertexSpecular();
    const auto &objShininess = obj.getVertexShininess();

    for (int triangleIdx = 0; triangleIdx < obj.getNumTriangles(); ++triangleIdx) {

        auto triangle = objTriangles[triangleIdx];

        int vIdx1 = triangle.getFirst();
        int vIdx2 = triangle.getSecond();
        int vIdx3 = triangle.getThird();

        QVector3D v1 = objVertices[vIdx1];
        QVector3D v2 = objVertices[vIdx2];
        QVector3D v3 = objVertices[vIdx3];

        QVector3D normal = objFaceNormals[triangleIdx];

        for (QVector3D v : {v1, v2, v3}) {
            vertices.append(v[0]);
            vertices.append(v[1]);
            vertices.append(v[2]);
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


    auto vao = QSharedPointer<QOpenGLVertexArrayObject>::create(nullptr);
    vao->create();
    vao->bind();

    vertexPositions->bind();
    mShaderProgram.setAttrPositionBuffer();
    vertexPositions->release();

    vertexNormals->bind();
    mShaderProgram.setAttrNormalBuffer();
    vertexNormals->release();

    vertexMaterials->bind();
    mShaderProgram.setAttrReflAmbientBuffer(0 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflDiffuseBuffer(1 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflSpecularBuffer(2 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrShininessBuffer(3 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    vertexMaterials->release();

    vao->release();


    mVAOs[&obj] = vao;
    mObjectVertexPositions[&obj] = vertexPositions;
    mObjectVertexNormals[&obj] = vertexNormals;
    mObjectVertexMaterials[&obj] = vertexMaterials;
    mNumVertices[&obj] = vertices.size() / 3;
}


#define REMOVE_INVALID_KEYS(validKeys, keyItr) while (keyItr.hasNext()) if(!validKeys.contains(keyItr.next().key())) keyItr.remove();

void SimpleTexturedRenderer::cleanMaps()
{
    QSet<const SimpleTexturedObject *> validKeys;

    for (const SimpleTexturedObject &obj : *mScene)
        validKeys.insert(&obj);


    auto keyItr1 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mVAOs)::mapped_type>(mVAOs);
    auto keyItr2 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexPositions)::mapped_type>(mObjectVertexPositions);
    auto keyItr3 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexNormals)::mapped_type>(mObjectVertexNormals);
    auto keyItr4 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mObjectVertexMaterials)::mapped_type>(mObjectVertexMaterials);
    auto keyItr5 = QMutableMapIterator<const SimpleTexturedObject *, decltype(mNumVertices)::mapped_type>(mNumVertices);

    // For each map, go through its keys and remove invalid ones.
    REMOVE_INVALID_KEYS(validKeys, keyItr1);
    REMOVE_INVALID_KEYS(validKeys, keyItr2);
    REMOVE_INVALID_KEYS(validKeys, keyItr3);
    REMOVE_INVALID_KEYS(validKeys, keyItr4);
    REMOVE_INVALID_KEYS(validKeys, keyItr5);
}

#undef REMOVE_INVALID_KEYS
