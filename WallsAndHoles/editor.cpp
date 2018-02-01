#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"

#include <QDockWidget>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>

Editor::Editor(QObject *parent)
    : QObject(parent)
    , mMainWindow(new QMainWindow())
    , mMap2Mesh(nullptr)
    , mTileMap(nullptr)
    , mMapView(new MapView(mTileMapSelectedRegion, mMainWindow))
    , mTileMapToolManager(new TileMapToolManager(this))
    , mToolBar(new QToolBar(mMainWindow))
{
    //Initiallize mMainWindow
    mMainWindow->setCentralWidget(mMapView);
    setUpMenuBar();
    mMainWindow->addToolBar(mToolBar);

    mToolBar->addAction(mTileMapToolManager->registerTool(QSharedPointer<AbstractTileMapTool>(new TileMapBrushTool(mTileMap)), "Brush Tool"));

    //Set up and add all dock widgets
    QDockWidget *dw = new QDockWidget("Mesh View", mMainWindow);
    mMeshViewContainer = new MeshViewContainer(dw);
    dw->setWidget(mMeshViewContainer);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);

    //Create widget connections
    connect(mMapView, &MapView::cellActivated,
            mTileMapToolManager, &TileMapToolManager::cellActivated);
    connect(mMapView, &MapView::cellClicked,
            mTileMapToolManager, &TileMapToolManager::cellClicked);
    connect(mMapView, &MapView::cellReleased,
            mTileMapToolManager, &TileMapToolManager::cellReleased);

    mMainWindow->showMaximized();
}

Editor::~Editor()
{
    delete mMainWindow;
}

void Editor::newMap()
{
    NewMapDialog nmd;
    nmd.exec();

    if (nmd.result.width != -1) {
        if (mTileMap)
            delete mTileMap;
        mTileMap = new TileMap(QSize(nmd.result.width, nmd.result.height), this);
        mTileMapToolManager->setTileMap(mTileMap);

        mMapView->createMap(mTileMap);

        if (mMap2Mesh != nullptr)
            delete mMap2Mesh;

        mMap2Mesh = new Map2Mesh(mTileMap, this);

        // TODO: It is inefficient to update the entire scene when just a part
        // of the map is updated.
        connect(mMap2Mesh, &Map2Mesh::mapMeshUpdated, this, &Editor::makeNewScene);

        // Note: Map2Mesh's mapUpdated() signal is emitted during its constructor,
        // but that is BEFORE the above connection is made. Therefore, updateScene()
        // must be called manually here.
        makeNewScene();
    }
}

void Editor::makeNewScene()
{
    QSharedPointer<Scene> scene = QSharedPointer<Scene>::create();

    for (auto&& obj : mMap2Mesh->getMeshes())
        scene->addObject(obj);

    mMeshViewContainer->setScene(scene);
}

void Editor::saveMap()
{

}

void Editor::loadMap()
{

}

void Editor::exportMapMesh()
{
    /*MeshViewContainer *meshViewContainer = findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export OBJ"), "/home", tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->saveMesh(fileName);
    */

    /*MeshViewContainer *meshViewContainer = findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("import OBJ"), "/home", tr("import Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->loadMesh(fileName);
    */
}

void Editor::setUpMenuBar()
{
    QMenuBar *menuBar = mMainWindow->menuBar();
    menuBar->clear();

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(tr("New Map"), this, &Editor::newMap);
    fileMenu->addAction(tr("Save Map"), this, &Editor::saveMap);
    fileMenu->addAction(tr("Load Map"), this, &Editor::loadMap);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Export Map Mesh"), this, &Editor::exportMapMesh);
}
