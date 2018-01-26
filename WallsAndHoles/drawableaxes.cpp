
#include <QVector4D>

#include "drawableaxes.h"

DrawableAxes::DrawableAxes()
{

}

void DrawableAxes::initializeGL() {
    initializeOpenGLFunctions();

    mProgram = QSharedPointer<QOpenGLShaderProgram>::create(nullptr);
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colors.vsh");
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colors.fsh");
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

    mPos = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    mPos->create();
    mPos->bind();
    mPos->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mPos->allocate(pos, 3 * 6 * sizeof(float));
    mPos->release();

    mColor = QSharedPointer<QOpenGLBuffer>::create(QOpenGLBuffer::VertexBuffer);
    mColor->create();
    mColor->bind();
    mColor->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mColor->allocate(col, 4 * 6 * sizeof(float));
    mColor->release();

    mVAO = QSharedPointer<QOpenGLVertexArrayObject>::create(nullptr);
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

}


void DrawableAxes::draw(QMatrix4x4 projection, QMatrix4x4 transformation) {
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
