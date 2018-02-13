#ifndef SIMPLETEXTUREDSHADER_H
#define SIMPLETEXTUREDSHADER_H

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLTexture>


/**
 * @brief Wrapper for the 'onelighttexture' GLSL program.
 */
class SimpleTexturedShader {
public:
    /**
     * @brief This constructor DOES NOT CREATE THE PROGRAM. Call create() before
     * using any other methods.
     */
    SimpleTexturedShader();

    /**
     * @brief Creates the program. Assumes an OpenGL context is bound.
     */
    void create();

    /**
     * @brief Binds the OpenGL program.
     */
    void bind();

    /**
     * @brief Unbinds the OpenGL program.
     */
    void release();

    /**
     * @brief Enables all relevant arrays.
     */
    void enableArrays();

    /**
     * @brief Disables all relevant arrays.
     */
    void disableArrays();



    /* Methods to bind buffers to attributes */
    void setAttrPositionBuffer(int offset = 0, int stride = 0);
    void setAttrNormalBuffer(int offset = 0, int stride = 0);
    void setAttrReflSpecularBuffer(int offset = 0, int stride = 0);
    void setAttrReflDiffuseBuffer(int offset = 0, int stride = 0);
    void setAttrReflAmbientBuffer(int offset = 0, int stride = 0);
    void setAttrShininessBuffer(int offset = 0, int stride = 0);
    void setAttrTexCoordsBuffer(int offset = 0, int stride = 0);

    /* Methods to set uniforms */
    void setUniformMVP(QMatrix4x4 mat);
    void setUniformPointToLight(QVector3D pos);
    void setUniformCameraPosition(QVector3D pos);
    void setUniformAmbientColor(QVector3D color);
    void setUniformSourceSpecularColor(QVector3D color);
    void setUniformSourceDiffuseColor(QVector3D color);


    void bindUniformTexture(QOpenGLTexture &texture);
    void releaseUniformTexture(QOpenGLTexture &texture);

protected:
    QSharedPointer<QOpenGLShaderProgram> mProgram;

    // Vertex attribute locations.
    int mAttrPosition;
    int mAttrNormal;
    int mAttrReflSpecular;
    int mAttrReflDiffuse;
    int mAttrReflAmbient;
    int mAttrShininess;
    int mAttrTexCoords;

    // Uniform locations.
    int mUnifMVP;
    int mUnifPointToLight;
    int mUnifCameraPosition;
    int mUnifAmbientColor;
    int mUnifSourceSpecularColor;
    int mUnifSourceDiffuseColor;
    int mUnifTexture;
};

#endif // SIMPLETEXTUREDSHADER_H
