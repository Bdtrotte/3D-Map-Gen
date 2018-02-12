#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"

#include "filltool.h"

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
    , mTileTemplateSetManager(new TileTemplateSetsManager(nullptr, this))
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
                            QSharedPointer<FillTool>::create(mMapView, mTileMap)
                            , "Fill Tool"));
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

    QDockWidget *tdw = new QDockWidget("Template Set View", mMainWindow);
    mTileTemplateSetsView = new TileTemplateSetsView(mTileTemplateSetManager, tdw);
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
    connect(mMapView, &MapView::cellHovered,
            mTileMapToolManager, &TileMapToolManager::cellHovered);
    connect(mMapView, &MapView::mouseExitedMap,
            mTileMapToolManager, &TileMapToolManager::mouseExitedMap);

    connect(mTileTemplateSetsView, &TileTemplateSetsView::tileTemplateChanged,
            mTileMapToolManager, &TileMapToolManager::tileTemplateChanged);

    mMainWindow->showMaximized();
}

Editor::~Editor()
{
    delete mMainWindow;
    delete mTileMap;
}

void Editor::newMap()
{
    NewMapDialog nmd;
    nmd.exec();

    if (nmd.accepted) {
        setTileMap(new TileMap(QSize(nmd.result.width, nmd.result.height), nmd.TileMapProperty));
    }else{
        return;
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
    // TODO : Save should be disabled if there is no map
    if(mTileMap == nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","TileMap doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    if(mTileMap->savePath().isEmpty()){
        mTileMap->setSavePath(QFileDialog::getSaveFileName(mMainWindow,
                                                           tr("Save Map"),
                                                           "/home/",
                                                           tr("Save Files (*.wah)")));
    }

    mTileTemplateSetManager->saveAllTileTemplateSets();

    XMLTool::saveTileMap(mTileMap, mTileTemplateSetManager->tileTemplateSets());
}

void Editor::loadMap()
{
    QString fileName = QFileDialog::getOpenFileName(mMainWindow,
                                                    tr("Open Map"),
                                                    "/home/",
                                                    tr("Open Files (*.wah)"));

    TileMap *tileMap = XMLTool::openTileMap(fileName, mTileTemplateSetManager);

    if (tileMap == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Fail to load TileMap!");
        messageBox.setFixedSize(500,200);
        return;
    }

    setTileMap(tileMap);
}

void Editor::closeMap()
{
    setTileMap(nullptr);
}

void Editor::exportMapMesh()
{
    if (mMeshViewContainer == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","MeshView doesn't exist!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(mMainWindow,
                                                    tr("Export OBJ"),
                                                    "/home/",
                                                    tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        mMeshViewContainer->saveMesh(fileName);
}

void Editor::setTileMap(TileMap *tileMap)
{
    TileMap *pre = mTileMap;
    mTileMap = tileMap;
    mTileMapToolManager->setTileMap(mTileMap);
    mMapView->createMap(mTileMap);
    mTileTemplateSetManager->setTileMap(mTileMap);
    if (mTileMap)
        mTileTemplateSetsView->setDefaultTileTemplateSet(mTileMap->defaultTileTemplateSet());
    else
        mTileTemplateSetsView->setDefaultTileTemplateSet(nullptr);

    delete mMap2Mesh;

    mMap2Mesh = new Map2Mesh(mTileMap, this);

    // TODO: It is inefficient to update the entire scene when just a part
    // of the map is updated.
    connect(mMap2Mesh, &Map2Mesh::mapMeshUpdated, this, &Editor::makeNewScene);

    // Note: Map2Mesh's mapUpdated() signal is emitted during its constructor,
    // but that is BEFORE the above connection is made. Therefore, updateScene()
    // must be called manually here.
    makeNewScene();

    // TODO SHOULD provide option to save old tileMap
    delete pre;
}

void Editor::setUpMenuBar()
{
    QMenuBar *menuBar = mMainWindow->menuBar();
    menuBar->clear();

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(tr("New Map"), this, &Editor::newMap);
    fileMenu->addAction(tr("Save Map"), this, &Editor::saveMap);
    fileMenu->addAction(tr("Load Map"), this, &Editor::loadMap);
    fileMenu->addAction(tr("Close Map"), this, &Editor::closeMap);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Export Map Mesh"), this, &Editor::exportMapMesh);
}
