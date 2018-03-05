#ifndef MESHVIEW_H
#define MESHVIEW_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMutex>

#include "abstractmeshviewcamera.h"
#include "toolmanager.h"
#include "objtools.h"

#include "meshviewcontainer.h"
#include "abstractrenderer.h"


class MeshView : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

    friend class MeshViewContainer;

public:
    explicit MeshView(QWidget *parent = 0);
    ~MeshView();

public slots:
    void load(QString path);
    void save(QString path);


    /**
     * @brief Schedules a paintGL() call on the OpenGL thread.
     */
    void scheduleRepaint();

    void makeContextCurrent();
    void doneContextCurrent();


    /**
     * @brief Cleans up all resources.
     */
    void cleanUp();

private slots:
    void cameraActivated(AbstractTool *tool, QString);

protected:
    /**
     * @brief Makes the MeshView use a new Renderer.
     * @param renderer  The Renderer that will be used to draw to the screen.
     */
    void setRenderer(QSharedPointer<AbstractRenderer> renderer);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


    /**
     * @brief This method should ONLY be called on the OpenGL thread.
     * It returns the appropriate renderer to be used, considering the
     * value of mNextRenderer.
     *
     * This function does not lock the mRendererMutex.
     *
     * @return The Renderer that should be used.
     */
    QSharedPointer<AbstractRenderer> getCurrentRenderer();



    /**
     * @brief The Renderer object that will be used to draw to the screen.
     */
    QSharedPointer<AbstractRenderer> mRenderer;

    /**
     * @brief nullptr if the Renderer does not need to change, else equal
     * to the new renderer.
     */
    QSharedPointer<AbstractRenderer> mNextRenderer;

    /**
     * @brief Mutex for updating the mRenderer variable. This ensures that the
     * mRenderer is not being used when it is changed.
     */
    QMutex mRendererMutex;


    // The Projection*View matrix.
    QMatrix4x4 mProjectionMatrix;


    // Controller for the camera.
    AbstractMeshViewCamera *mCamera;

    // The tool manager. This will send mouse events to the appropriate tool.
    ToolManager *mTools;


    /**
     * @brief The context this MeshView was initialized with. This variable is only
     * stored to disconnect its aboutToBeDestroyed() signal when the MeshView is
     * itself destroyed. This pointer is not owned by the MeshView, but should
     * still be valid in ~MeshView() (although this cannot be guaranteed---darn you,
     * Qt's lack of shared pointers!).
     */
    QOpenGLContext *mContext;
};

#endif // MESHVIEW_H
