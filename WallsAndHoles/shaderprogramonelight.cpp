#include "shaderprogramonelight.h"

ShaderProgramOneLight::ShaderProgramOneLight() {

}


void ShaderProgramOneLight::create() {
    mProgram = QSharedPointer<QOpenGLShaderProgram>::create(nullptr);
    mProgram->create();
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/onelight.vsh");
    mProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/onelight.fsh");
    mProgram->link();

    mAttrPosition = mProgram->attributeLocation("vPosition");
    mAttrNormal = mProgram->attributeLocation("vNormal");
    mAttrReflSpecular = mProgram->attributeLocation("vReflSpecular");
    mAttrReflDiffuse = mProgram->attributeLocation("vReflDiffuse");
    mAttrReflAmbient = mProgram->attributeLocation("vReflAmbient");
    mAttrShininess = mProgram->attributeLocation("vShininess");

    mUnifMVP = mProgram->uniformLocation("mvp");
    mUnifLightPosition = mProgram->uniformLocation("uLightPosition");
    mUnifCameraPosition = mProgram->uniformLocation("uCameraPosition");
    mUnifAmbientColor = mProgram->uniformLocation("uAmbientColor");
    mUnifSourceSpecularColor = mProgram->uniformLocation("uSourceSpecularColor");
    mUnifSourceDiffuseColor = mProgram->uniformLocation("uSourceDiffuseColor");
}

void ShaderProgramOneLight::bind() {
    mProgram->bind();
}

void ShaderProgramOneLight::release() {
    mProgram->release();
}


void ShaderProgramOneLight::enableArrays() {
    mProgram->enableAttributeArray(mAttrPosition);
    mProgram->enableAttributeArray(mAttrNormal);
    mProgram->enableAttributeArray(mAttrReflSpecular);
    mProgram->enableAttributeArray(mAttrReflDiffuse);
    mProgram->enableAttributeArray(mAttrReflAmbient);
    mProgram->enableAttributeArray(mAttrShininess);
}

void ShaderProgramOneLight::disableArrays() {
    mProgram->disableAttributeArray(mAttrPosition);
    mProgram->disableAttributeArray(mAttrNormal);
    mProgram->disableAttributeArray(mAttrReflSpecular);
    mProgram->disableAttributeArray(mAttrReflDiffuse);
    mProgram->disableAttributeArray(mAttrReflAmbient);
    mProgram->disableAttributeArray(mAttrShininess);
}


void ShaderProgramOneLight::setAttrPositionBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrPosition, GL_FLOAT, offset, 3, stride);
}

void ShaderProgramOneLight::setAttrNormalBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrNormal, GL_FLOAT, offset, 3, stride);
}

void ShaderProgramOneLight::setAttrReflSpecularBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflSpecular, GL_FLOAT, offset, 1, stride);
}

void ShaderProgramOneLight::setAttrReflDiffuseBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflDiffuse, GL_FLOAT, offset, 1, stride);
}

void ShaderProgramOneLight::setAttrReflAmbientBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrReflAmbient, GL_FLOAT, offset, 1, stride);
}

void ShaderProgramOneLight::setAttrShininessBuffer(int offset, int stride) {
    mProgram->setAttributeBuffer(mAttrShininess, GL_FLOAT, offset, 1, stride);
}


void ShaderProgramOneLight::setUniformMVP(QMatrix4x4 mat) {
    mProgram->setUniformValue(mUnifMVP, mat);
}

void ShaderProgramOneLight::setUniformLightPosition(QVector3D pos) {
    mProgram->setUniformValue(mUnifLightPosition, pos);
}

void ShaderProgramOneLight::setUniformCameraPosition(QVector3D pos) {
    mProgram->setUniformValue(mUnifCameraPosition, pos);
}

void ShaderProgramOneLight::setUniformAmbientColor(QVector3D color) {
    mProgram->setUniformValue(mUnifAmbientColor, color);
}

void ShaderProgramOneLight::setUniformSourceSpecularColor(QVector3D color) {
    mProgram->setUniformValue(mUnifSourceSpecularColor, color);
}

void ShaderProgramOneLight::setUniformSourceDiffuseColor(QVector3D color) {
    mProgram->setUniformValue(mUnifSourceDiffuseColor, color);
}
