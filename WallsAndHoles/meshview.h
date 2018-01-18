#ifndef MESHVIEW_H
#define MESHVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>

#include <scene.h>

#define SHADER_VERTEX_POS "qt_Vertex"
#define SHADER_MVP "qt_ModelViewProjectionMatrix"

namespace Ui {
class MeshView;
}

class MeshView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MeshView(std::shared_ptr<Scene> scene, QWidget *parent = 0);
    ~MeshView();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    // Loads the vertex data for the scene onto the GPU. Assumes mScene is not nullptr.
    // After this, it is safe to call paintGL().
    void loadVAO();


    // Shader program.
    QOpenGLShaderProgram mBasicProgram;

    // VAO and buffers for rendering.
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVertexPositions;
    QOpenGLBuffer mTriangleIndices;

    // Scene object that is rendered.
    std::shared_ptr<Scene> mScene;

    // The Projection*View matrix.
    QMatrix4x4 mProjectionMatrix;

    // The translation that should be applied to the MVP.
    QVector3D mTranslation;


    // The position the mouse was at the last time.
    QPoint mMouseLast;


private:
    Ui::MeshView *ui;
};

#endif // MESHVIEW_H
