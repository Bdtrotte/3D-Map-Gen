#include "editor.h"

#include "newmapdialog.h"
#include "clearmapdialog.h"
#include "meshviewcontainer.h"

#include <QDockWidget>

Editor::Editor(QObject *parent)
    : QObject(parent)
    , mMainWindow(new MainWindow())
    , mMapView(new MapView(mMainWindow))
    , mToolBar(new QToolBar(mMainWindow))
    , mTileMap(new TileMap(QSize(1,1)))
{
    mMainWindow->show();
    mMainWindow->setCentralWidget(mMapView);

    QAction *newMap = mToolBar->addAction("New Map");
    connect(newMap, &QAction::triggered, this, &Editor::createNewMap);

    QAction *clearMap = mToolBar->addAction("Clear Map");
    connect(clearMap, &QAction::triggered, this, &Editor::clearMapView);

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
        mTileMap->resizeMap(QSize(nmd.result.width,nmd.result.height));
        mMapView->createMap(nmd.result.width, nmd.result.height);
    }
}

void Editor::clearMapView(){
    ClearMapDialog cmd;
    cmd.exec();
    if(cmd.accepted){
        mMapView->clear();
        mTileMap->resizeMap(QSize(1,1));
    }
}
