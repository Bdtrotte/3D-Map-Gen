#include <QTimer>
#include <QMutexLocker>


#include "meshview.h"
#include "ui_meshview.h"
#include "objtools.h"

#include "meshviewcameralikeblender.h"


MeshView::MeshView(QWidget *parent) :
    QOpenGLWidget(parent),
    mNextRenderer(nullptr),
    mUseScheduled(false),
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
    QMutexLocker rendererMutex(&mRendererMutex);


    // Disconnect the previous renderer's signals.
    if (!mRenderer.isNull())
        mRenderer->disconnect(this);

    // Don't actually change mRenderer here -- its destructor must be called
    // on the OpenGL thread.
    mNextRenderer = renderer;

    connect(renderer.data(), &AbstractRenderer::repaintNeeded, this, &MeshView::scheduleRepaint);
    connect(renderer.data(), &AbstractRenderer::openGLThreadNeeded, this, &MeshView::scheduleUse);
}


bool MeshView::event(QEvent *e)
{
    if (e->type() == QEvent::User) {

        QMutexLocker useScheduledLocker(&mUseScheduledMutex);
        mUseScheduled = false;
        useScheduledLocker.unlock();

        QMutexLocker rendererMutex(&mRendererMutex);

        makeCurrent();
        auto renderer = getCurrentRenderer();

        if (!renderer.isNull())
            renderer->useGL();
        doneCurrent();

        rendererMutex.unlock();

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
        mRenderer = mNextRenderer;
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
    QMutexLocker useScheduledLocker(&mUseScheduledMutex);

    if (!mUseScheduled) {
        mUseScheduled = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }
}


void MeshView::cleanUp()
{
    QMutexLocker rendererMutex(&mRendererMutex);

    // Make the context current. At this stage, the MeshView still has the old context,
    // but it may not be bound here.
    makeCurrent();


    if (!mRenderer.isNull())
        mRenderer->cleanUp();


    // Release the context.
    doneCurrent();
}



// This method is called whenever there is a new context. If there was an old context
// before, it would have been destroyed and its aboutToBeDestroyed() signal would have
// been emitted.
// This happens whenever the QOpenGLWidget is reparented (or docked/undocked).
void MeshView::initializeGL()
{
    initializeOpenGLFunctions();
    QMutexLocker rendererMutex(&mRendererMutex);

    auto renderer = getCurrentRenderer();
    if (!renderer.isNull())
        renderer->initializeGL();


    // Connect the context's aboutToBeDestroyed() signal to this view's cleanUp() signal
    // so that data is cleaned up before the context is destroyed.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MeshView::cleanUp);
}

void MeshView::paintGL()
{
    QMutexLocker rendererMutex(&mRendererMutex);

    auto renderer = getCurrentRenderer();

    if (!renderer.isNull()) {
        QMatrix4x4 transform = mCamera->getTransformationMatrix();
        QMatrix4x4 mvp = mProjectionMatrix * transform;
        renderer->paint(mvp, mCamera->getPosition());
    }
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
    auto renderer = getCurrentRenderer();
    SharedOBJModel obj = renderer->exportOBJ();
    obj->saveOBJ(path);
//qDebug() << "MeshView::save() is deprecated. Nothing is done.";
}


