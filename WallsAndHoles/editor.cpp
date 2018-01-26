#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"

#include <QDockWidget>

Editor::Editor(QObject *parent)
    : QObject(parent)
    , mMainWindow(new MainWindow())
    , mMapView(new MapView(mMainWindow))
    , mToolBar(new QToolBar(mMainWindow))
{
    mMainWindow->show();
    mMainWindow->setCentralWidget(mMapView);

    QAction *newMap = mToolBar->addAction("New Map");
    connect(newMap, &QAction::triggered, this, &Editor::createNewMap);

    mMainWindow->addToolBar(mToolBar);

    QDockWidget *dw = new QDockWidget("Mesh View", mMainWindow);
    MeshViewContainer *mvc = new MeshViewContainer(dw);
    dw->setWidget(mvc);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);
}

void Editor::createNewMap()
{
    NewMapDialog nmd;
    nmd.exec();
    if (nmd.result.width != -1) {
        mMapView->createMap(nmd.result.width, nmd.result.height);
    }
}
