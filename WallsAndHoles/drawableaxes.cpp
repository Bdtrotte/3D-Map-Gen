
#include <QVector4D>

#include "drawableaxes.h"

DrawableAxes::DrawableAxes()
    : initialized(false)
{

}

DrawableAxes::~DrawableAxes() {
    delete mVAO;
    delete mPos;
    delete mColor;
}

void DrawableAxes::initialize() {
    initializeOpenGLFunctions();

    mProgram = QSharedPointer<QOpenGLShaderProgram>::create();
    mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colors.vsh");
    mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colors.fsh");
    mProgram->link();

    float length = 10;

    float pos[] = {0, 0, 0,
                   length, 0, 0,
                   0, 0, 0,
                   0, length, 0,
                   0, 0, 0,
                   0, 0, length};

    float col[] = {1, 0, 0, 1,
                   1, 0, 0, 1,
                   0, 1, 0, 1,
                   0, 1, 0, 1,
                   0, 0, 1, 1,
                   0, 0, 1, 1};

    mPos = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mPos->create();
    mPos->bind();
    mPos->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mPos->allocate(pos, 3 * 6 * sizeof(float));
    mPos->release();

    mColor = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mColor->create();
    mColor->bind();
    mColor->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mColor->allocate(col, 4 * 6 * sizeof(float));
    mColor->release();

    mVAO = new QOpenGLVertexArrayObject();
    mVAO->create();
    mVAO->bind();

    mProgram->bind();
    mPos->bind();
    mProgram->setAttributeBuffer("vPos", GL_FLOAT, 0, 3);
    mPos->release();

    mColor->bind();
    mProgram->setAttributeBuffer("vColor", GL_FLOAT, 0, 4);
    mColor->release();
    mProgram->release();

    mVAO->release();


    initialized = true;
}


void DrawableAxes::draw(QMatrix4x4 projection, QMatrix4x4 transformation) {

    // Initialize on the first call. This sets up shaders and buffers.
    if (!initialized)
        initialize();


    mProgram->bind();
    mProgram->setUniformValue("mvp", projection * transformation);

    mVAO->bind();

    mProgram->enableAttributeArray("vPos");
    mProgram->enableAttributeArray("vColor");
    glDrawArrays(GL_LINES, 0, 6);
    mProgram->disableAttributeArray("vColor");
    mProgram->disableAttributeArray("vPos");

    mVAO->release();

    mProgram->release();
}
