#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"
#include "tilemapselectiontool.h"
#include "propertybrowser.h"
#include "mappropertymanager.h"
#include "tilematerialview.h"

#include "filltool.h"

#include "linebrushtool.h"
#include "rectbrushtool.h"
#include "ellipsebrushtool.h"

#include "abstractscene.h"
#include "abstractrenderer.h"

#include <QDockWidget>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QListView>
#include <QShortcut>

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


    //Set up and add all dock widgets
    QDockWidget *meshViewDock = new QDockWidget("Mesh View", mMainWindow);
    mMeshViewContainer = new MeshViewContainer(meshViewDock);
    meshViewDock->setWidget(mMeshViewContainer);

    QDockWidget *templateDock = new QDockWidget("Template Set View", mMainWindow);
    mTileTemplateSetsView = new TileTemplateSetsView(mTileTemplateSetManager, templateDock);
    templateDock->setWidget(mTileTemplateSetsView);

    QDockWidget *propBrowserDock = new QDockWidget("Property Browser View", mMainWindow);
    mPropertyBrowser = new PropertyBrowser(mMainWindow);
    propBrowserDock->setWidget(mPropertyBrowser);

    QDockWidget *materialDock = new QDockWidget("Material View", mMainWindow);
    TileMaterialView *materialView = new TileMaterialView(mMainWindow);
    materialDock->setWidget(materialView);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, meshViewDock);
    mMainWindow->addDockWidget(Qt::LeftDockWidgetArea, templateDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, propBrowserDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, materialDock);


    // Add tools.
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapBrushTool>::create(mMapView->previewItem())
                            , "Brush Tool"
                            , QIcon("://icons/22x22/brush.png")));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<FillTool>::create(mMapView->previewItem())
                            , "Fill Tool"
                            , QIcon("://icons/22x22/stock-tool-bucket-fill.png")));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<LineBrushTool>::create(mMapView->previewItem())
                            , "Line Tool"
                            , QIcon("://icons/22x22/line.png")));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<RectBrushTool>::create(mMapView->previewItem())
                            , "Rect Tool"
                            , QIcon(":/icons/22x22/rectangle-fill.png")));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<EllipseBrushTool>::create(mMapView->previewItem())
                            , "Ellipse Tool"
                            , QIcon(":/icons/22x22/ellipse-fill.png")));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapSelectionTool>::create(mPropertyBrowser, mMapView->previewItem())
                            , "Selection Tool"
                            , QIcon("://icons/22x22/mouse.png")));

    //Sets up the context toolBar
    mToolBar->addSeparator();
    mToolBar->addWidget(mTileMapToolManager->contextToolBar());

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

    if (nmd.exec()) {
        setTileMap(new TileMap(QSize(nmd.result().width, nmd.result().height),
                               nmd.result().isIndoorMap,
                               nmd.result().hasCeiling));
    }
}


void Editor::saveMap()
{
    Q_ASSERT(mTileMap != nullptr);

    if (mTileMap->savePath().isEmpty()) {
        QString savePath = QFileDialog::getSaveFileName(mMainWindow,
                                                        tr("Save Map"),
                                                        "/home/",
                                                        tr("Save Files (*.wah)"));

        if (savePath.isEmpty())
            return;
        mTileMap->setSavePath(savePath);
    }

    mTileTemplateSetManager->saveAllTileTemplateSets();

    if (XMLTool::saveTileMap(mTileMap, mTileTemplateSetManager->tileTemplateSets()) != XMLTool::NoError) {
        mTileMap->setSavePath("");
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Fail to save TileMap!");
        messageBox.setFixedSize(500,200);
    }
}

void Editor::saveMapAs()
{
    Q_ASSERT(mTileMap != nullptr);

    mTileTemplateSetManager->saveAllTileTemplateSets();

    QString prePath = mTileMap->savePath();
    QString savePath = QFileDialog::getSaveFileName(mMainWindow,
                                                    tr("Save Map"),
                                                    "/home/",
                                                    tr("Save Files (*.wah)"));

    if (savePath.isEmpty())
        return;
    mTileMap->setSavePath(savePath);

    if (XMLTool::saveTileMap(mTileMap, mTileTemplateSetManager->tileTemplateSets()) != XMLTool::NoError) {
        mTileMap->setSavePath(prePath);
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Fail to save TileMap!");
        messageBox.setFixedSize(500,200);
    }
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

void Editor::viewMapProperties()
{
    mPropertyBrowser->setPropertyManager(new MapPropertyManager(mTileMap));
}

void Editor::setTileMap(TileMap *tileMap)
{
    if (mTileMap == tileMap) return;

    // TODO : currently TileMap doesn't keep track of if it needs to be saved or not,
    //        so in some cases this check might not be needed.
    if (mTileMap) {
        QMessageBox mb;
        mb.setText("Do you want to save the current Tile Map?");
        mb.addButton("Save", QMessageBox::AcceptRole);
        mb.addButton("Don't Save", QMessageBox::AcceptRole);
        mb.addButton("Cancel", QMessageBox::RejectRole);

        switch (mb.exec()) {
        case 0:
            saveMap();
            break;
        case 2:
            return;
        }
    }

    TileMap *pre = mTileMap;
    mTileMap = tileMap;
    mTileMapToolManager->setTileMap(mTileMap);
    mMapView->createMap(mTileMap);
    mTileTemplateSetManager->setTileMap(mTileMap);
    mPropertyBrowser->clear();
    setMapDependantActionsEnabled(mTileMap != nullptr);

    if (mTileMap) {
        mTileTemplateSetsView->setDefaultTileTemplateSet(mTileMap->defaultTileTemplateSet());
    } else {
        mTileTemplateSetsView->setDefaultTileTemplateSet(nullptr);
        mPropertyBrowser->clear();
    }


    delete mMap2Mesh;
    mMap2Mesh = new Map2Mesh(mTileMap, this);

    SharedAbstractScene scene = mMap2Mesh->getScene();
    QSharedPointer<AbstractRenderer> renderer = scene->getRenderer();

    mMeshViewContainer->setRenderer(renderer);

    renderer->requestUpdate();

    delete pre;
}

void Editor::setUpMenuBar()
{
    QMenuBar *menuBar = mMainWindow->menuBar();
    menuBar->clear();

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
                                fileMenu->addAction(tr("New Map"), this, &Editor::newMap
                                                    , Qt::CTRL + Qt::Key_N);
    mMapDependantActions.append(fileMenu->addAction(tr("Save Map"), this, &Editor::saveMap
                                                    , Qt::CTRL + Qt::Key_S));
    mMapDependantActions.append(fileMenu->addAction(tr("Save Map As"), this, &Editor::saveMapAs
                                                    , Qt::CTRL + Qt::SHIFT + Qt::Key_S));
                                fileMenu->addAction(tr("Load Map"), this, &Editor::loadMap
                                                    , Qt::CTRL + Qt::Key_O);
    mMapDependantActions.append(fileMenu->addAction(tr("Close Map"), this, &Editor::closeMap
                                                    , Qt::CTRL + Qt::Key_W));
    fileMenu->addSeparator();
    mMapDependantActions.append(fileMenu->addAction(tr("Export Map Mesh"), this, &Editor::exportMapMesh
                                                    , Qt::CTRL + Qt::Key_E));


    QMenu *mapMenu = menuBar->addMenu(tr("Map"));
    mMapDependantActions.append(mapMenu->addAction(tr("View Map Properties"), this, &Editor::viewMapProperties));

    setMapDependantActionsEnabled(false);
}
