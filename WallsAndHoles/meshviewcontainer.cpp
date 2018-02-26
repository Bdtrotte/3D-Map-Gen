#include "meshviewcontainer.h"
#include "ui_meshviewcontainer.h"

#include "meshview.h"
#include "objtools.h"

MeshViewContainer::MeshViewContainer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    // Create the MeshView.
    mMeshView = new MeshView(this);
    mMeshView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(mMeshView);

    mMeshView->activateTool("camera");
}

void MeshViewContainer::setRenderer(QSharedPointer<AbstractRenderer> renderer) {
    mMeshView->setRenderer(renderer);
}

void MeshViewContainer::saveMesh(QString path){
    mMeshView = findChild<MeshView *>();
    mMeshView->save(path);
}
void MeshViewContainer::loadMesh(QString path){
    mMeshView = findChild<MeshView *>();
    mMeshView->load(path);
}
