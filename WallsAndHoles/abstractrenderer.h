#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

#include <QOpenGLFunctions>
#include <QMatrix4x4>

// For std::function
#include <functional>


/**
 * @brief Something that renders a 3D scene to the screen using OpenGL.
 */
class AbstractRenderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT

public:

    virtual ~AbstractRenderer() {}

    /**
     * @brief Uses OpenGL commands to draw to the frame buffer. Nothing is assumed
     * about the previous contents of the frame (i.e. glClear does not need to be called
     * before this), and the OpenGL state should not change as a result of calling this
     * function.
     *
     * It is assumed that initializeGL() has been called for the current OpenGL context.
     *
     * @param vpMatrix  The model-view-projection matrix that should be used.
     * @param camPos    The world-space coordinates of the camera.
     */
    virtual void paint(QMatrix4x4 mvpMatrix, QVector3D camPos) = 0;


    /**
     * @brief Initializes OpenGL-related details.
     */
    void initializeGL()
    {
        initializeOpenGLFunctions();

        initializeRenderer();
    }

public slots:

    /**
     * @brief Calling this slot makes the renderer update itself when possible.
     */
    void requestUpdate() { emit repaintNeeded(); }


    /**
     * @brief This slot should clean up all resources associated to the renderer.
     */
    virtual void cleanUp() = 0;

signals:
    /**
     * @brief Emitted when the renderer has updates to make to its image.
     */
    void repaintNeeded();


    void makeContextCurrent();
    void doneContextCurrent();

protected:
    /**
     * @brief Helper method to loop glGetError() and print out all current errors.
     *
     * Prints nothing if there are no errors, but otherwise prints the names of the errors.
     *
     * @return Returns true if an error was detected and false otherwise.
     */
    bool checkGLErrors()
    {
        bool hadError = false;

        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            switch (error) {
            case GL_INVALID_ENUM:                   qDebug() << "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  qDebug() << "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              qDebug() << "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  qDebug() << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                  qDebug() << "GL_OUT_OF_MEMORY"; break;
            case GL_STACK_UNDERFLOW:                qDebug() << "GL_STACK_UNDERFLOW"; break;
            case GL_STACK_OVERFLOW:                 qDebug() << "GL_STACK_OVERFLOW"; break;
            default:                                qDebug() << "Unknown error."; break;
            }

            hadError = true;
        }

        return hadError;
    }


    /**
     * @brief Used to initialize renderer parameters that require an OpenGL context
     * to be bound.
     */
    virtual void initializeRenderer() = 0;

};

#endif // ABSTRACTRENDERER_H
