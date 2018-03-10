#include "editor.h"

#include "newmapdialog.h"
#include "meshviewcontainer.h"
#include "tilemapbrushtool.h"
#include "tilemapselectiontool.h"
#include "propertybrowser.h"
#include "mappropertymanager.h"

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
    , mUndoGroup(new QUndoGroup(this))
    , mTileMapUndoStack(new QUndoStack(mUndoGroup))
    , mMap2Mesh(nullptr)
    , mTileMap(nullptr)
    , mTileTemplateSetManager(new TileTemplateSetsManager(nullptr, this))
    , mMapViewContainer(new MapViewContainer(mMainWindow))
    , mTileMapToolManager(new TileMapToolManager(this))
    , mToolBar(new QToolBar(mMainWindow))
{
    //Set the application name
    QCoreApplication::setOrganizationName("WAH");
    QCoreApplication::setApplicationName("Walls and Holes");

    //Initiallize mMainWindow
    mMainWindow->setCentralWidget(mMapViewContainer);
    setUpMenuBar();
    mMainWindow->addToolBar(mToolBar);

    //Set up and add all dock widgets
    QDockWidget *meshViewDock = new QDockWidget("Mesh View", mMainWindow);
    mMeshViewContainer = new MeshViewContainer(meshViewDock);
    meshViewDock->setWidget(mMeshViewContainer);
    meshViewDock->setObjectName("Mesh View Dock");

    QDockWidget *templateDock = new QDockWidget("Template Set View", mMainWindow);
    mTileTemplateSetsView = new TileTemplateSetsView(mTileTemplateSetManager, templateDock);
    templateDock->setWidget(mTileTemplateSetsView);
    templateDock->setObjectName("Template Dock");

    QDockWidget *propBrowserDock = new QDockWidget("Property Browser View", mMainWindow);
    mPropertyBrowser = new PropertyBrowser(mMainWindow);
    propBrowserDock->setWidget(mPropertyBrowser);
    propBrowserDock->setObjectName("Prop Browser Dock");

    QDockWidget *materialDock = new QDockWidget("Material View", mMainWindow);
    mMaterialView = new TileMaterialView(mMainWindow);
    materialDock->setWidget(mMaterialView);
    materialDock->setObjectName("Material Dock");

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, meshViewDock);
    mMainWindow->addDockWidget(Qt::LeftDockWidgetArea, templateDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, propBrowserDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, materialDock);

    // Add tools.
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapBrushTool>::create(mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Brush Tool"
                            , QIcon("://images/icons/22x22/brush.png")
                            , QKeySequence(Qt::Key_B)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<FillTool>::create(mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Fill Tool"
                            , QIcon("://images/icons/22x22/stock-tool-bucket-fill.png")
                            , QKeySequence(Qt::Key_F)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<LineBrushTool>::create(mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Line Tool"
                            , QIcon("://images/icons/22x22/line.png")
                            , QKeySequence(Qt::Key_L)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<RectBrushTool>::create(mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Rect Tool"
                            , QIcon("://images/icons/22x22/rectangle-fill.png")
                            , QKeySequence(Qt::Key_R)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<EllipseBrushTool>::create(mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Ellipse Tool"
                            , QIcon("://images/icons/22x22/ellipse-fill.png")
                            , QKeySequence(Qt::Key_E)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapSelectionTool>::create(mPropertyBrowser, mMapViewContainer->mapView()->previewItem(), mTileMapUndoStack)
                            , "Selection Tool"
                            , QIcon("://images/icons/22x22/mouse.png")
                            , QKeySequence(Qt::Key_S)));

    // Make TileMap undo stack active by default. This is only necessary
    // until relevant undo stacks are set active automatically (e.g.
    // when a window receives focus).
    mTileMapUndoStack->setActive(true);

    //Sets up the context toolBar
    mToolBar->addSeparator();
    mToolBar->addWidget(mTileMapToolManager->contextToolBar());
    mToolBar->setObjectName("Tool Bar");

    //Create widget connections
    connect(mMapViewContainer->mapView(), &MapView::cellActivated,
            mTileMapToolManager, &TileMapToolManager::cellActivated);
    connect(mMapViewContainer->mapView(), &MapView::cellClicked,
            mTileMapToolManager, &TileMapToolManager::cellClicked);
    connect(mMapViewContainer->mapView(), &MapView::cellReleased,
            mTileMapToolManager, &TileMapToolManager::cellReleased);
    connect(mMapViewContainer->mapView(), &MapView::cellHovered,
            mTileMapToolManager, &TileMapToolManager::cellHovered);
    connect(mMapViewContainer->mapView(), &MapView::mouseExitedMap,
            mTileMapToolManager, &TileMapToolManager::mouseExitedMap);

    connect(mTileTemplateSetsView, &TileTemplateSetsView::tileTemplateChanged,
            mTileMapToolManager, &TileMapToolManager::tileTemplateChanged);

    mMainWindow->showMaximized();
    loadSettings();
}

Editor::~Editor()
{
    saveSettings();
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
                                                        mSavePath,
                                                        tr("Save Files (*.wah)"));

        if (savePath.isEmpty())
            return;
        mTileMap->setSavePath(savePath);
        mSavePath = savePath;
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
                                                    mSavePath,
                                                    tr("Save Files (*.wah)"));

    if (savePath.isEmpty())
        return;
    mTileMap->setSavePath(savePath);
    mSavePath = savePath;

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
                                                    mSavePath,
                                                    tr("Open Files (*.wah)"));

    TileMap *tileMap = XMLTool::openTileMap(fileName, mTileTemplateSetManager);

    if (tileMap == nullptr) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Fail to load TileMap!");
        messageBox.setFixedSize(500,200);
        return;
    }
    mSavePath = fileName;
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
                                                    mExportPath,
                                                    tr("Export Files (*.obj)"));

    if(!fileName.isEmpty())
        mMeshViewContainer->saveMesh(fileName);
    mExportPath = fileName;
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

    // Clear the TileMap undo stack since the actions in this stack are now invalid.
    mTileMapUndoStack->clear();

    TileMap *pre = mTileMap;
    mTileMap = tileMap;
    mTileMapToolManager->setTileMap(mTileMap);
    mMapViewContainer->mapView()->setMap(mTileMap);
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


    // Create undo and redo actions.
    QAction *undoAction = mUndoGroup->createUndoAction(this, tr("Undo"));
    QAction *redoAction = mUndoGroup->createRedoAction(this, tr("Redo"));
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction->setShortcut(QKeySequence::Redo);

    QMenu *editMenu = menuBar->addMenu(tr("Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    QMenu *mapMenu = menuBar->addMenu(tr("Map"));
    mMapDependantActions.append(mapMenu->addAction(tr("View Map Properties"), this, &Editor::viewMapProperties));


    setMapDependantActionsEnabled(false);
}

void Editor::loadSettings()
{
    QSettings settings;

    mMainWindow->restoreState(settings.value("windowState").toByteArray());

    if(!settings.value("tileMap").toString().isNull())
        setTileMap(XMLTool::openTileMap(settings.value("tileMap").toString(), mTileTemplateSetManager));
    else
        setTileMap(nullptr);

    //Loading saving and export paths
    mSavePath = settings.value("savePath", QString("/home/")).toString();
    mExportPath = settings.value("exportPath", QString("/home/")).toString();

}

void Editor::saveSettings()
{
    QSettings settings;

    //Saves the windowstate
    if(mMainWindow != nullptr)
        settings.setValue("windowState", mMainWindow->saveState());

    //Saves the open Tilemap
    if(mTileMap)
        settings.setValue("tileMap", mTileMap->savePath());
    else
        settings.setValue("tileMap", QString());

    //Save and Export paths
    settings.setValue("savePath", mSavePath);
    settings.setValue("exportPath", mExportPath);

}
