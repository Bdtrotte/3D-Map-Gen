#include "meshviewcontainer.h"
#include "ui_meshviewcontainer.h"

#include "meshview.h"
#include "objtools.h"

MeshViewContainer::MeshViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewContainer)
{
    ui->setupUi(this);

    // Find the MeshView child widget.
    mMeshView = findChild<MeshView *>();
}

MeshViewContainer::~MeshViewContainer() {
    delete ui;
}

void MeshViewContainer::setRenderer(QSharedPointer<AbstractRenderer> renderer) {
    mMeshView->setRenderer(renderer);
}

void MeshViewContainer::on_toolSelection_currentIndexChanged(int index) {
    switch (index) {
    case 0:
        mMeshView->activateTool("nothing");
        break;
    case 1:
        mMeshView->activateTool("camera");
        break;
    }
}
