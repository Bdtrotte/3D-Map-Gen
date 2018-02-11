#include <QTimer>

#include "meshview.h"
#include "ui_meshview.h"
#include "objtools.h"

#include "meshviewcameralikeblender.h"


MeshView::MeshView(QWidget *parent) :
    QOpenGLWidget(parent),
    mNextRenderer(nullptr),
    useScheduled(false),
    ui(new Ui::MeshView)
{
    ui->setupUi(this);

    mCamera = QSharedPointer<MeshViewCameraLikeBlender>::create();

    connect(mCamera.data(), &AbstractMeshViewCamera::changed, this, &MeshView::scheduleRepaint);

    mTools = ToolManagerP::create();
    mTools->registerTool(mCamera, "camera");
}

MeshView::~MeshView() {
    delete ui;
}


void MeshView::setRenderer(QSharedPointer<AbstractRenderer> renderer) {
    mRendererMutex.lock();


    // Disconnect the previous renderer's signals.
    if (!mRenderer.isNull())
        mRenderer->disconnect(this);

    // Don't actually change mRenderer here -- its destructor must be called
    // on the OpenGL thread.
    mNextRenderer = renderer;

    connect(renderer.data(), &AbstractRenderer::repaintNeeded, this, &MeshView::scheduleRepaint);
    connect(renderer.data(), &AbstractRenderer::openGLThreadNeeded, this, &MeshView::scheduleUse);

    mRendererMutex.unlock();
}


bool MeshView::event(QEvent *e)
{
    if (e->type() == QEvent::User) {
        useScheduledMutex.lock();
        useScheduled = false;
        useScheduledMutex.unlock();

        mRendererMutex.lock();

        auto renderer = getCurrentRenderer();

        if (!renderer.isNull())
            renderer->useGL();

        mRendererMutex.unlock();

        e->accept();
        return true;
    }

    return QOpenGLWidget::event(e);
}


void MeshView::mousePressEvent(QMouseEvent *event) {
    mTools->mousePressEvent(event);
}

void MeshView::mouseMoveEvent(QMouseEvent *event) {
    mTools->mouseMoveEvent(event);
}

void MeshView::mouseReleaseEvent(QMouseEvent *event) {
    mTools->mouseReleaseEvent(event);
}

void MeshView::wheelEvent(QWheelEvent *event) {
    mTools->wheelEvent(event);
}


void MeshView::activateTool(QString name) {
    mTools->activateTool(name);
}


QSharedPointer<AbstractRenderer> MeshView::getCurrentRenderer()
{
    if (!mNextRenderer.isNull()) {
        qDebug() << "Trying to change mRenderer.";
        mRenderer = mNextRenderer;
        qDebug() << "Succeeded changing mRenderer.";
        mNextRenderer = nullptr;

        mRenderer->initializeGL();
    }

    return mRenderer;
}


void MeshView::scheduleRepaint()
{
    update();
}

void MeshView::scheduleUse()
{
    useScheduledMutex.lock();

    if (!useScheduled) {
        qDebug() << "scheduleUse()";

        useScheduled = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }

    useScheduledMutex.unlock();
}



// This is called WHENEVER THERE IS A NEW CONTEXT.
// A new context can be created if the meshview is undocked. This is important!
void MeshView::initializeGL()
{
    mRendererMutex.lock();

    auto renderer = getCurrentRenderer();
    if (!renderer.isNull())
        renderer->initializeGL();

    mRendererMutex.unlock();
}

void MeshView::paintGL()
{
    mRendererMutex.lock();

    auto renderer = getCurrentRenderer();

    if (!renderer.isNull()) {
        QMatrix4x4 transform = mCamera->getTransformationMatrix();
        QMatrix4x4 mvp = mProjectionMatrix * transform;
        renderer->paint(mvp, mCamera->getPosition());
    }

    mRendererMutex.unlock();
}

void MeshView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(90, ((float) w) / h, 0.1, 100);

    update();
}

void MeshView::load(QString path){
//    QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();
//    scene->addObject(loadOBJ(path));
//    setScene(scene);

    qDebug() << "MeshView::load() is deprecated. Nothing is done.";
}

void MeshView::save(QString path){
//    QVector<QSharedPointer<RenderableObject>> object = mScene->getAllObjects();
//    if(object.size()<1){
//        qDebug() << "Fail to save mesh: scene is empty";
//    }
//    saveOBJ(path, object[0]);

    qDebug() << "MeshView::save() is deprecated. Nothing is done.";
}


