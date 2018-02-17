#include "simpletexturedshader.h"

SimpleTexturedShader::SimpleTexturedShader()
{

}


void SimpleTexturedShader::create()
{
    mProgram = QSharedPointer<QOpenGLShaderProgram>::create(nullptr);
    mProgram->create();
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/onelighttexture.vsh");
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/onelighttexture.fsh");
    mProgram->link();

    mAttrPosition = mProgram->attributeLocation("vPosition");
    mAttrNormal = mProgram->attributeLocation("vNormal");
    mAttrReflSpecular = mProgram->attributeLocation("vReflSpecular");
    mAttrReflDiffuse = mProgram->attributeLocation("vReflDiffuse");
    mAttrReflAmbient = mProgram->attributeLocation("vReflAmbient");
    mAttrShininess = mProgram->attributeLocation("vShininess");
    mAttrTexCoords = mProgram->attributeLocation("vTexCoords");

    mUnifMVP = mProgram->uniformLocation("mvp");
    mUnifPointToLight = mProgram->uniformLocation("uPointToLight");
    mUnifCameraPosition = mProgram->uniformLocation("uCameraPosition");
    mUnifAmbientColor = mProgram->uniformLocation("uAmbientColor");
    mUnifSourceSpecularColor = mProgram->uniformLocation("uSourceSpecularColor");
    mUnifSourceDiffuseColor = mProgram->uniformLocation("uSourceDiffuseColor");
    mUnifTexture = mProgram->uniformLocation("uTexture");
}

void SimpleTexturedShader::destroy()
{
    mProgram = nullptr;
}

void SimpleTexturedShader::bind() {
    mProgram->bind();
}

void SimpleTexturedShader::release() {
    mProgram->release();
}


void SimpleTexturedShader::enableArrays() {
    mProgram->enableAttributeArray(mAttrPosition);
    mProgram->enableAttributeArray(mAttrNormal);
    mProgram->enableAttributeArray(mAttrTexCoords);
    mProgram->enableAttributeArray(mAttrReflSpecular);
    mProgram->enableAttributeArray(mAttrReflDiffuse);
    mProgram->enableAttributeArray(mAttrReflAmbient);
    mProgram->enableAttributeArray(mAttrShininess);
}

void SimpleTexturedShader::disableArrays() {
    mProgram->disableAttributeArray(mAttrPosition);
    mProgram->disableAttributeArray(mAttrNormal);
    mProgram->disableAttributeArray(mAttrTexCoords);
    mProgram->disableAttributeArray(mAttrReflSpecular);
    mProgram->disableAttributeArray(mAttrReflDiffuse);
    mProgram->disableAttributeArray(mAttrReflAmbient);
    mProgram->disableAttributeArray(mAttrShininess);
}


void SimpleTexturedShader::setAttrPositionBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrPosition, GL_FLOAT, offset, 3, stride);
}

void SimpleTexturedShader::setAttrNormalBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrNormal, GL_FLOAT, offset, 3, stride);
}

void SimpleTexturedShader::setAttrReflSpecularBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflSpecular, GL_FLOAT, offset, 1, stride);
}

void SimpleTexturedShader::setAttrReflDiffuseBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflDiffuse, GL_FLOAT, offset, 1, stride);
}

void SimpleTexturedShader::setAttrReflAmbientBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflAmbient, GL_FLOAT, offset, 1, stride);
}

void SimpleTexturedShader::setAttrShininessBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrShininess, GL_FLOAT, offset, 1, stride);
}

void SimpleTexturedShader::setAttrTexCoordsBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrTexCoords, GL_FLOAT, offset, 2, stride);
}


void SimpleTexturedShader::setUniformMVP(QMatrix4x4 mat) {
    mProgram->setUniformValue(mUnifMVP, mat);
}

void SimpleTexturedShader::setUniformPointToLight(QVector3D pos) {
    mProgram->setUniformValue(mUnifPointToLight, pos);
}

void SimpleTexturedShader::setUniformCameraPosition(QVector3D pos) {
    mProgram->setUniformValue(mUnifCameraPosition, pos);
}

void SimpleTexturedShader::setUniformAmbientColor(QVector3D color) {
    mProgram->setUniformValue(mUnifAmbientColor, color);
}

void SimpleTexturedShader::setUniformSourceSpecularColor(QVector3D color) {
    mProgram->setUniformValue(mUnifSourceSpecularColor, color);
}

void SimpleTexturedShader::setUniformSourceDiffuseColor(QVector3D color) {
    mProgram->setUniformValue(mUnifSourceDiffuseColor, color);
}


void SimpleTexturedShader::bindUniformTexture(QOpenGLTexture &texture) {
    texture.bind(0);
    mProgram->setUniformValue(mUnifTexture, (GLuint)0); // The passed value is the active texture unit.
}

void SimpleTexturedShader::releaseUniformTexture(QOpenGLTexture &texture) {
    texture.release(0);
}
