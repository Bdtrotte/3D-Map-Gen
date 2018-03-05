#include <QVBoxLayout>

#include "meshviewcontainer.h"
#include "ui_meshviewcontainer.h"

#include "meshview.h"
#include "objtools.h"

MeshViewContainer::MeshViewContainer(QWidget *parent)
    : QWidget(parent)
    , mMeshView(new MeshView(this))
    , mToolBar(new QToolBar(this))
{
    mMeshView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mToolBar->addAction(mMeshView->mTools->getAction("Default"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(mToolBar);
    layout->addWidget(mMeshView);

    setLayout(layout);
}

void MeshViewContainer::setRenderer(QSharedPointer<AbstractRenderer> renderer)
{
    mMeshView->setRenderer(renderer);
}

QAction *MeshViewContainer::addCamera(AbstractMeshViewCamera *camera, QString name, QIcon icon, QKeySequence ks)
{
    mToolBar->addAction(mMeshView->addCamera(camera, name, icon, ks));
}

void MeshViewContainer::saveMesh(QString path){
    mMeshView = findChild<MeshView *>();
    mMeshView->save(path);
}
void MeshViewContainer::loadMesh(QString path){
    mMeshView = findChild<MeshView *>();
    mMeshView->load(path);
}
