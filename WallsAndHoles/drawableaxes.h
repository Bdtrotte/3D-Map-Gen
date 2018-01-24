#ifndef DRAWABLEAXES_H
#define DRAWABLEAXES_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "drawableglobject.h"

/**
 * @brief An object that may be added to a Scene to display the XYZ axes.
 *
 * DrawableAxes always draws itself at (0, 0, 0). It does not disable depth
 * testing, so the axes could be completely covered up by a large object.
 */
class DrawableAxes : public DrawableGLObject, public QOpenGLFunctions {
public:
    DrawableAxes();
    ~DrawableAxes();

    void draw(QMatrix4x4 projection, QMatrix4x4 transformation) override;

protected:

    QOpenGLVertexArrayObject *mVAO;
    QOpenGLBuffer *mPos;
    QOpenGLBuffer *mColor;

    QSharedPointer<QOpenGLShaderProgram> mProgram;


    // Whether initialize() has been called yet.
    bool initialized;

    /**
     * @brief Called during first draw() call to initialize buffers and shaders.
     */
    void initialize();
};

#endif // DRAWABLEAXES_H
