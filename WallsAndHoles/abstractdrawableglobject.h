#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <QMatrix4x4>

/**
 * @brief Abstract class for objects with special OpenGL rendering.
 *
 * Abstract class containing a single draw() function for customized OpenGL rendering.
 * The purpose of this class is to allow a scene to contain non-mesh widgets (like a
 * widget for visualizing the axes).
 */
class AbstractDrawableGLObject {
public:
    virtual ~AbstractDrawableGLObject() {}
    
    /**
     * @brief Called when this object is to be used with a new OpenGL context.
     * Must be called before the first draw() call in the context.
     */
    virtual void initializeGL() = 0;

    /**
     * @brief Performs OpenGL calls to draw the object, assuming a context is bound.
     * @param projection        The projection matrix used for the meshes.
     * @param transformation    The transformation matrix used for the meshes. The
     *                          purpose of separating this from the projection
     *                          is to allow a user to extract the translation and
     *                          rotation components.
     *
     * The gl_Position of a vertex V is given by (projection * transformation * V).
     */
    virtual void draw(QMatrix4x4 projection, QMatrix4x4 transformation) = 0;
};

#endif // DRAWABLEOBJECT_H
