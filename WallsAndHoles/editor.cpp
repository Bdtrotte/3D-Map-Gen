#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"

#include "linebrushtool.h"
#include "rectbrushtool.h"
#include "ellipsebrushtool.h"

#include <QDockWidget>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

#include <QListView>

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

    // Add tools.

    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapBrushTool>::create(mTileMap)
                            , "Brush Tool"));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<LineBrushTool>::create(mMapView, mTileMap)
                            , "Line Tool"));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<RectBrushTool>::create(mMapView, mTileMap)
                            , "Rect Tool"));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<EllipseBrushTool>::create(mMapView, mTileMap)
                            , "Ellipse Tool"));

    //Set up and add all dock widgets
    QDockWidget *dw = new QDockWidget("Mesh View", mMainWindow);
    mMeshViewContainer = new MeshViewContainer(dw);
    dw->setWidget(mMeshViewContainer);

    //TMP testing templatesets model view
    QDockWidget *tdw = new QDockWidget("Template Set View", mMainWindow);
    mTileTemplateSetsView = new TileTemplateSetsView(tdw);
    tdw->setWidget(mTileTemplateSetsView);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, dw);
    mMainWindow->addDockWidget(Qt::LeftDockWidgetArea, tdw);

    //Create widget connections
    connect(mMapView, &MapView::cellActivated,
            mTileMapToolManager, &TileMapToolManager::cellActivated);
    connect(mMapView, &MapView::cellClicked,
            mTileMapToolManager, &TileMapToolManager::cellClicked);
    connect(mMapView, &MapView::cellReleased,
            mTileMapToolManager, &TileMapToolManager::cellReleased);

    connect(mTileTemplateSetsView, &TileTemplateSetsView::tileTemplateChanged,
            mTileMapToolManager, &TileMapToolManager::tileTemplateChanged);

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
        mTileMap = new TileMap(QSize(nmd.result.width, nmd.result.height), nmd.TileMapProperty, this);

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
    if(mTileMap==nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","TileMap doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }
    //mTileMap->setDepend(QSharedPointer<TileTemplateSet>(mTileTemplateSets[0]));
    //mTileMap->updateDepend();
    if(mTileMap->savePath().isEmpty()){
        mTileMap->setSavePath(QFileDialog::getSaveFileName(mMainWindow,
            tr("Save Map"), "/home", tr("Save Files (*.xml)")));
    }
    for(SharedTileTemplateSet set: mTileMap->dependencies()){
        if(set->savePath().isEmpty()){
            set->setSavePath(QFileDialog::getSaveFileName(mMainWindow,
                tr("Save Templates"), "/home", tr("Save Files (*.xml)")));
        }
    }
    XMLTool::saveTileMap(QSharedPointer<TileMap>(mTileMap));
}

void Editor::loadMap()
{
    QString fileName = QFileDialog::getOpenFileName(mMainWindow,
        tr("Open Map"), "/home", tr("Open Files (*.xml)"));

    SharedTileMap tileMap = XMLTool::openTileMap(fileName);
    if(tileMap==nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Fail to load TileMap!");
        messageBox.setFixedSize(500,200);
        return;
    }
    mTileMap = tileMap.data();
    if(!mTileMap->dependencies().isEmpty())
        //mTileTemplateSets[0] = (mTileMap->dependencies()[0]);
    mTileMapToolManager->setTileMap(mTileMap);
    mMapView->createMap(mTileMap);
}

void Editor::exportMapMesh()
{

    MeshViewContainer *meshViewContainer = mMainWindow->findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(mMainWindow,
        tr("Export OBJ"), "/home", tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        meshViewContainer->saveMesh(fileName);

/*
    MeshViewContainer *meshViewContainer = mMainWindow->findChild<MeshViewContainer *>();

    if (meshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(mMainWindow,
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
