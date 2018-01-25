#include <QTimer>

#include "meshview.h"
#include "ui_meshview.h"

#include "meshviewcameralikeblender.h"


MeshView::MeshView(QWidget *parent) :
    QOpenGLWidget(parent),
    mVertexPositions(QOpenGLBuffer::VertexBuffer),
    mTriangleIndices(QOpenGLBuffer::IndexBuffer),
    mShouldReloadBuffers(false),
    ui(new Ui::MeshView)
{
    ui->setupUi(this);

    mCamera = QSharedPointer<MeshViewCameraLikeBlender>::create();

    mTools = ToolManagerP::create();
    mTools->registerTool(mCamera, "camera");
}

MeshView::~MeshView()
{
    delete ui;

    // TODO: Make sure OpenGL related objects are freed.
}


void MeshView::setScene(QSharedPointer<Scene> scene) {
    mScene = scene;
    mShouldReloadBuffers = true;
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


// Assumes an OpenGL context is bound, mBasicProgram is set up, and mScene != nullptr.
void MeshView::loadVAO() {
    QVector<GLfloat> vertices;
    QVector<GLuint> indices;

    for (auto obj : mScene->getAllObjects()) {
        const QVector<QVector3D>& vdata = obj->getVertexData();
        for (int i = 0; i < vdata.size(); ++i) {
            vertices.append(vdata[i][0]);
            vertices.append(vdata[i][1]);
            vertices.append(vdata[i][2]);
        }

        const QVector<unsigned int>& triangleIndices = obj->getTriangleIndices();
        GLuint index = indices.size(); //Can't render multiple object without this
        for (int i = 0; i < triangleIndices.size(); ++i)
            indices.append(index+triangleIndices[i]);
    }

    mVertexPositions.create();
    mVertexPositions.bind();
    mVertexPositions.setUsagePattern(QOpenGLBuffer::DynamicDraw); // may be updated in real-time in the future
    mVertexPositions.allocate(vertices.data(), vertices.size() * sizeof(GLfloat));
    mVertexPositions.release();

    mTriangleIndices.create();
    mTriangleIndices.bind();
    mTriangleIndices.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mTriangleIndices.allocate(indices.data(), indices.size() * sizeof(GLuint));
    mTriangleIndices.release();

    mVAO.create();
    mVAO.bind();

    mVertexPositions.bind();
    mBasicProgram.setAttributeBuffer(SHADER_VERTEX_POS, GL_FLOAT, 0, 3);
    mVertexPositions.release();

    mVAO.release();

    mShouldReloadBuffers = false;
}

void MeshView::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    // Create the shader program.
    mBasicProgram.create();
    mBasicProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vsh");
    mBasicProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.fsh");
    mBasicProgram.link();

    // Start outputting frame updates.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

void MeshView::paintGL() {

    if (mShouldReloadBuffers)
        loadVAO();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mScene != nullptr) {
        /******* Draw meshes. *******/
        // Set program.
        mBasicProgram.bind();

        // Set the matrix.
        QMatrix4x4 transform = mCamera->getTransformationMatrix();
        QMatrix4x4 mvp = mProjectionMatrix * transform;
        mBasicProgram.setUniformValue(SHADER_MVP, mvp);



        // Set arrays.
        mVAO.bind();
        mTriangleIndices.bind();

        // Draw.
        mBasicProgram.enableAttributeArray(SHADER_VERTEX_POS);
        glDrawElements(GL_TRIANGLES, mTriangleIndices.size()/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        mBasicProgram.disableAttributeArray(SHADER_VERTEX_POS);

        // Unset arrays.
        mTriangleIndices.release();
        mVAO.release();

        // Unset program.
        mBasicProgram.release();

        /******* Draw widgets. *******/
        for (QSharedPointer<AbstractDrawableGLObject> drawable : mScene->getAllDrawables())
            drawable->draw(mProjectionMatrix, transform);
    }
}

void MeshView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(90, ((float) w) / h, 0.1, 30);
}



