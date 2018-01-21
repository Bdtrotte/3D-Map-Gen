#include <QTimer>

#include "meshview.h"
#include "ui_meshview.h"

#include "meshviewcameralikeblender.h"


MeshView::MeshView(QSharedPointer<Scene> scene, QWidget *parent) :
    QOpenGLWidget(parent),
    mVertexPositions(QOpenGLBuffer::VertexBuffer),
    mTriangleIndices(QOpenGLBuffer::IndexBuffer),
    mScene(scene),
    ui(new Ui::MeshView)
{
    ui->setupUi(this);

    mCamera = new MeshViewCameraLikeBlender();
}

MeshView::~MeshView()
{
    delete ui;
}



void MeshView::mousePressEvent(QMouseEvent *event) {
    mCamera->mousePressEvent(event);
}

void MeshView::mouseMoveEvent(QMouseEvent *event) {
    mCamera->mouseMoveEvent(event);
}




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
        for (int i = 0; i < triangleIndices.size(); ++i)
            indices.append(triangleIndices[i]);
    }


    mVertexPositions.create();
    mVertexPositions.setUsagePattern(QOpenGLBuffer::DynamicDraw); // may be updated in real-time in the future
    mVertexPositions.bind();
    mVertexPositions.allocate(vertices.data(), vertices.size() * sizeof(GLfloat));

    mTriangleIndices.create();
    mTriangleIndices.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mTriangleIndices.bind();
    mTriangleIndices.allocate(indices.data(), indices.size() * sizeof(GLuint));

    mVAO.create();
    mVAO.bind();

    mVertexPositions.bind();
    mBasicProgram.setAttributeBuffer(SHADER_VERTEX_POS, GL_FLOAT, 0, 3);
    mVertexPositions.release();

    mVAO.release();
}

void MeshView::initializeGL() {
    initializeOpenGLFunctions();

    // Create the shader program.
    mBasicProgram.create();
    mBasicProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vsh");
    mBasicProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.fsh");
    mBasicProgram.link();
    mBasicProgram.bind();

    loadVAO();

    // Start outputting frame updates.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

void MeshView::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    // Set program.
    mBasicProgram.bind();

    // Set the matrix.
    QMatrix4x4 mvp = mCamera->getTransformationMatrix();
    mvp = mProjectionMatrix * mvp;
    mBasicProgram.setUniformValue(SHADER_MVP, mvp);



    // Set arrays.
    mVAO.bind();
    mTriangleIndices.bind();

    // Draw.
    mBasicProgram.enableAttributeArray(SHADER_VERTEX_POS);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    mBasicProgram.disableAttributeArray(SHADER_VERTEX_POS);

    // Unset arrays.
    mTriangleIndices.release();
    mVAO.release();

    // Unset program.
    mBasicProgram.release();
}

void MeshView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    // TODO: For now, the projection matrix is always a simple orthogonal projection.
    mProjectionMatrix.setToIdentity();
//    mProjectionMatrix.ortho(-1, 1, 1, -1, 0.1, 100);
    mProjectionMatrix.perspective(90, ((float) w) / h, 0.1, 30);
}



