#include "meshviewcontainer.h"
#include "ui_meshviewcontainer.h"

#include "meshview.h"
#include "scene.h"
#include "objtools.h"

MeshViewContainer::MeshViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewContainer)
{
    ui->setupUi(this);

    // Create a test scene.
    QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();
    scene->addObject(loadOBJ(":/assets/cube.obj"));

    // Find the MeshView child widget and set the scene.
    mMeshView = findChild<MeshView *>();
    mMeshView->setScene(scene);
}

MeshViewContainer::~MeshViewContainer() {
    delete ui;
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
