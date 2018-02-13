#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <QObject>
#include <QQueue>
#include <QMutex>

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


    /**
     * @brief Called sometime after the openGLThreadNeeded() signal is emitted.
     * It is assumed that the correct OpenGL context is current.
     */
    void useGL()
    {
        mFunctionsForOpenGLMutex.lock();

        while (!mFunctionsForOpenGL.isEmpty())
            // Dequeues and invokes a function.
            mFunctionsForOpenGL.dequeue()();

        mFunctionsForOpenGLMutex.unlock();
    }

public slots:

    /**
     * @brief Calling this slot makes the renderer update itself when possible.
     */
    void requestUpdate() { emit repaintNeeded(); }

signals:
    /**
     * @brief Emitted when the renderer has updates to make to its image.
     */
    void repaintNeeded();


    /**
     * @brief Emitted when the renderer needs the thread with the correct OpenGL context
     * to do something.
     */
    void openGLThreadNeeded();

protected:


    /**
     * @brief Queues the given function to be called on a thread with the correct OpenGL
     * context. Arguments may optionally be passed.
     *
     * Example: callFunctionOnOpenGL(&MySubtype::myFunction, this, 1, 2);
     *
     * Note that "this" needs to be passed as the first argument to member functions.
     */
    template< typename Func, typename... Args >
    void callFunctionOnOpenGL(Func&& func, Args&&... arguments)
    {
        mFunctionsForOpenGLMutex.lock();

        std::function<void()> bound = std::bind(func, arguments...);
        mFunctionsForOpenGL.enqueue(bound);

        mFunctionsForOpenGLMutex.unlock();

        emit openGLThreadNeeded();
    }


    /**
     * @brief Helper method to loop glGetError() and print out all current errors.
     *
     * Prints nothing if there are no errors, but otherwise prints the names of the errors.
     */
    void checkGLErrors()
    {
        GLenum error = GL_NO_ERROR;

        do {
            error = glGetError();
            if (error != GL_NO_ERROR) {
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
            }
        } while (error != GL_NO_ERROR);
    }


    /**
     * @brief Used to initialize renderer parameters that require an OpenGL context
     * to be bound.
     */
    virtual void initializeRenderer() = 0;



private:
    /**
     * @brief A queue of functions that need to be invoked on an OpenGL thread.
     */
    QQueue<std::function<void()>> mFunctionsForOpenGL;

    /**
     * @brief A mutex for adding functions to the above queue in a thread-safe manner.
     */
    QMutex mFunctionsForOpenGLMutex;

};

#endif // ABSTRACTRENDERER_H
