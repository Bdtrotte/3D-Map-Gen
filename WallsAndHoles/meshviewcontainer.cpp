#include <QVBoxLayout>

#include "meshviewcontainer.h"

#include "meshview.h"
#include "meshviewcameralikeblender.h"
#include "objtools.h"

MeshViewContainer::MeshViewContainer(QWidget *parent)
    : QWidget(parent)
    , mMeshView(new MeshView(this))
    , mToolBar(new QToolBar(this))
{
    mMeshView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    addCamera(new MeshViewCameraLikeBlender(), "Default")->setChecked(true);

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
    QAction *cam = mMeshView->mTools->registerTool(camera, name, icon, ks);

    mToolBar->addAction(cam);

    return cam;
}
