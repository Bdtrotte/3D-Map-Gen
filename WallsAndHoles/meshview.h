#ifndef MESHVIEW_H
#define MESHVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>

#include "scene.h"
#include "abstractmeshviewcamera.h"
#include "toolmanager.h"
#include "objtools.h"


#define SHADER_VERTEX_POS "qt_Vertex"
#define SHADER_MVP "qt_ModelViewProjectionMatrix"

namespace Ui {
class MeshView;
}

class MeshView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MeshView(QWidget *parent = 0);
    ~MeshView();

    void setScene(QSharedPointer<Scene> scene);

public slots:
    /**
     * @brief Activates the given tool in the tool manager.
     * @param name The name of the tool.
     */
    void activateTool(QString name);
    void load(QString path);
    void save(QString path);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    // Loads the vertex data for the scene onto the GPU. Assumes mScene is not nullptr.
    // After this, it is safe to call paintGL().
    void loadVAO();


    // Shader program.
    QSharedPointer<QOpenGLShaderProgram> mBasicProgram;

    // VAO and buffers for rendering.
    QSharedPointer<QOpenGLVertexArrayObject> mVAO;
    QSharedPointer<QOpenGLBuffer> mVertexPositions;
    QSharedPointer<QOpenGLBuffer> mTriangleIndices;

    // Scene object that is rendered.
    QSharedPointer<Scene> mScene;

    // This variable is just used for synchronization. mScene should not be changed
    // in parallel with the paint loop.
    QSharedPointer<Scene> mNextScene;

    // The Projection*View matrix.
    QMatrix4x4 mProjectionMatrix;


    // Controller for the camera.
    QSharedPointer<AbstractMeshViewCamera> mCamera;

    // The tool manager. This will send mouse events to the appropriate tool.
    ToolManagerP mTools;

    // True when Scene related buffers need to be reloaded.
    bool mShouldReloadScene;

private:
    Ui::MeshView *ui;
};

#endif // MESHVIEW_H
