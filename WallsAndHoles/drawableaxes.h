#ifndef DRAWABLEAXES_H
#define DRAWABLEAXES_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "abstractdrawableglobject.h"

/**
 * @brief An object that may be added to a Scene to display the XYZ axes.
 *
 * DrawableAxes always draws itself at (0, 0, 0). It does not disable depth
 * testing, so the axes could be completely covered up by a large object.
 */
class DrawableAxes : public AbstractDrawableGLObject, public QOpenGLFunctions {
public:
    DrawableAxes();

    void initializeGL() override;
    void draw(QMatrix4x4 projection, QMatrix4x4 transformation) override;

protected:

    QSharedPointer<QOpenGLVertexArrayObject> mVAO;
    QSharedPointer<QOpenGLBuffer> mPos;
    QSharedPointer<QOpenGLBuffer> mColor;

    QSharedPointer<QOpenGLShaderProgram> mProgram;
};

#endif // DRAWABLEAXES_H
