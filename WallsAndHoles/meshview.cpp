#include <QTimer>

#include "meshview.h"
#include "ui_meshview.h"

#include "meshviewcameralikeblender.h"


MeshView::MeshView(QWidget *parent) :
    QOpenGLWidget(parent),
    mShouldReloadScene(false),
    ui(new Ui::MeshView)
{
    ui->setupUi(this);

    mCamera = QSharedPointer<MeshViewCameraLikeBlender>::create();

    mTools = ToolManagerP::create();
    mTools->registerTool(mCamera, "camera");


    // Start outputting frame updates.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

MeshView::~MeshView() {
    delete ui;
}


void MeshView::setScene(QSharedPointer<Scene> scene) {
    mNextScene = scene;

    // The reason loadVAO() is not called here is because the appropriate
    // OpenGL context may not be bound.
    mShouldReloadScene = true;
}


void MeshView::mousePressEvent(QMouseEvent *event) {
    mTools->mousePressEvent(event);
}

void MeshView::mouseMoveEvent(QMouseEvent *event) {
    mTools->mouseMoveEvent(event);
}

void MeshView::mouseReleaseEvent(QMouseEvent *event) {
    mTools->mouseReleaseEvent(event);
}

void MeshView::wheelEvent(QWheelEvent *event) {
    mTools->wheelEvent(event);
}


void MeshView::activateTool(QString name) {
    mTools->activateTool(name);
}


// Assumes an OpenGL context is bound, mShaderProgram is set up, and mScene != nullptr.
void MeshView::loadVAO() {
    mShouldReloadScene = false;

    QVector<GLfloat> vertices;
    QVector<GLfloat> normals;
    QVector<GLfloat> materials;
    QVector<GLuint> indices;

    for (auto obj : mScene->getAllObjects()) {
        const QVector<QVector3D>& vdata = obj->getVertexData();
        const QVector<QVector3D>& ndata = obj->getVertexNormals();
        const QVector<MeshMaterial>& mdata = obj->getVertexMaterials();

        for (int i = 0; i < vdata.size(); ++i) {
            vertices.append(vdata[i][0]);
            vertices.append(vdata[i][1]);
            vertices.append(vdata[i][2]);

            normals.append(ndata[i][0]);
            normals.append(ndata[i][1]);
            normals.append(ndata[i][2]);

            materials.append(mdata[i].getReflAmbient());
            materials.append(mdata[i].getReflDiffuse());
            materials.append(mdata[i].getReflSpecular());
            materials.append(mdata[i].getShininess());
        }


        const QVector<unsigned int>& triangleIndices = obj->getTriangleIndices();
        GLuint index = indices.size(); //Can't render multiple object without this
        for (int i = 0; i < triangleIndices.size(); ++i)
            indices.append(index+triangleIndices[i]);
    }

    mVertexPositions = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    mVertexPositions->create();
    mVertexPositions->bind();
    mVertexPositions->setUsagePattern(QOpenGLBuffer::DynamicDraw); // may be updated in real-time in the future
    mVertexPositions->allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    mVertexPositions->release();

    mVertexNormals = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    mVertexNormals->create();
    mVertexNormals->bind();
    mVertexNormals->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mVertexNormals->allocate(normals.data(), normals.size() * sizeof(GLfloat));
    mVertexNormals->release();

    mVertexMaterials = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    mVertexMaterials->create();
    mVertexMaterials->bind();
    mVertexMaterials->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mVertexMaterials->allocate(materials.data(), materials.size() * sizeof(GLfloat));
    mVertexMaterials->release();

    mTriangleIndices = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::IndexBuffer);
    mTriangleIndices->create();
    mTriangleIndices->bind();
    mTriangleIndices->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mTriangleIndices->allocate(indices.data(), indices.size() * sizeof(GLuint));
    mTriangleIndices->release();

    mVAO = QSharedPointer<QOpenGLVertexArrayObject>::create(nullptr);
    mVAO->create();
    mVAO->bind();

    mVertexPositions->bind();
    mShaderProgram.setAttrPositionBuffer();
    mVertexPositions->release();

    mVertexNormals->bind();
    mShaderProgram.setAttrNormalBuffer();
    mVertexNormals->release();

    mVertexMaterials->bind();
    mShaderProgram.setAttrReflAmbientBuffer(0 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflDiffuseBuffer(1 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrReflSpecularBuffer(2 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mShaderProgram.setAttrShininessBuffer(3 * sizeof(GLfloat), 4 * sizeof(GLfloat));
    mVertexMaterials->release();

    mVAO->release();
}

// This is called WHENEVER THERE IS A NEW CONTEXT.
// A new context can be created if the meshview is undocked. This is important!
void MeshView::initializeGL() {
    initializeOpenGLFunctions();

    // This will free the previously loaded program automatically.
    mShaderProgram.create();

    // If mScene exists, the buffers should be reloaded now.
    mShouldReloadScene = mScene != nullptr;

    // Initialize GL for all objects in the scene.
    if (mScene != nullptr)
        mScene->initializeGL();
}

void MeshView::paintGL() {

    if (mShouldReloadScene) {
        mScene = mNextScene;

        mScene->initializeGL();
        if (!mScene.isNull())
            loadVAO();
    }


    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!mScene.isNull()) {
        /******* Draw meshes. *******/
        // Set program.
        mShaderProgram.bind();

        // Set the matrix.
        QMatrix4x4 transform = mCamera->getTransformationMatrix();
        QMatrix4x4 mvp = mProjectionMatrix * transform;
        mShaderProgram.setUniformMVP(mvp);

        // Set the lighting uniforms.
        QVector3D camPos = mCamera->getPosition();

        mShaderProgram.setUniformLightPosition(camPos);
        mShaderProgram.setUniformCameraPosition(camPos);
        mShaderProgram.setUniformAmbientColor(QVector3D(0.1, 0.1, 0.2));
        mShaderProgram.setUniformSourceDiffuseColor(QVector3D(0.3, 0.3, 0.2));
        mShaderProgram.setUniformSourceSpecularColor(QVector3D(0.3, 0.3, 0.2));


        // Set arrays.
        mVAO->bind();
        mTriangleIndices->bind();

        // Draw.
        mShaderProgram.enableArrays();
        glDrawElements(GL_TRIANGLES, mTriangleIndices->size()/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        mShaderProgram.disableArrays();

        // Unset arrays.
        mTriangleIndices->release();
        mVAO->release();

        // Unset program.
        mShaderProgram.release();

        /******* Draw widgets. *******/
        for (QSharedPointer<AbstractDrawableGLObject> drawable : mScene->getAllDrawables())
            drawable->draw(mProjectionMatrix, transform);
    }

    // Clean up for anything else using the same context.
    glDisable(GL_DEPTH_TEST);
}

void MeshView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(90, ((float) w) / h, 0.1, 30);
}

void MeshView::load(QString path){
    QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();
    scene->addObject(loadOBJ(path));
    qDebug() << "new scene created...now set it";
    setScene(scene);
}

void MeshView::save(QString path){
    QVector<RenderableObjectP> object = mScene->getAllObjects();
    if(object.size()<1){
        qDebug() << "Fail to save mesh: scene is empty";
    }
    saveOBJ(path, object[0]);
}


