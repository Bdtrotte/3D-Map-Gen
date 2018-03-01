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
#include <QCloseEvent>
#include <algorithm>

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
    //Set the application name
    QCoreApplication::setOrganizationName("WAH");
    QCoreApplication::setApplicationName("Walls and Holes");

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
    mMaterialView = new TileMaterialView(mMainWindow);
    materialDock->setWidget(mMaterialView);

    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, meshViewDock);
    mMainWindow->addDockWidget(Qt::LeftDockWidgetArea, templateDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, propBrowserDock);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea, materialDock);

    //Save keep track of where the dockWidgets are
    connect(meshViewDock, &QDockWidget::dockLocationChanged, this, &Editor::saveSettings);
    connect(templateDock, &QDockWidget::dockLocationChanged, this, &Editor::saveSettings);
    connect(propBrowserDock, &QDockWidget::dockLocationChanged, this, &Editor::saveSettings);
    connect(materialDock, &QDockWidget::dockLocationChanged, this, &Editor::saveSettings);

    // Add tools.
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapBrushTool>::create(mMapView->previewItem())
                            , "Brush Tool"
                            , QIcon("://icons/22x22/brush.png")
                            , QKeySequence(Qt::Key_B)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<FillTool>::create(mMapView->previewItem())
                            , "Fill Tool"
                            , QIcon("://icons/22x22/stock-tool-bucket-fill.png")
                            , QKeySequence(Qt::Key_F)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<LineBrushTool>::create(mMapView->previewItem())
                            , "Line Tool"
                            , QIcon("://icons/22x22/line.png")
                            , QKeySequence(Qt::Key_L)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<RectBrushTool>::create(mMapView->previewItem())
                            , "Rect Tool"
                            , QIcon(":/icons/22x22/rectangle-fill.png")
                            , QKeySequence(Qt::Key_R)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<EllipseBrushTool>::create(mMapView->previewItem())
                            , "Ellipse Tool"
                            , QIcon(":/icons/22x22/ellipse-fill.png")
                            , QKeySequence(Qt::Key_E)));
    mToolBar->addAction(mTileMapToolManager->registerMapTool(
                            QSharedPointer<TileMapSelectionTool>::create(mPropertyBrowser, mMapView->previewItem())
                            , "Selection Tool"
                            , QIcon("://icons/22x22/mouse.png")
                            , QKeySequence(Qt::Key_S)));


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

    loadSettings();

    mMainWindow->showMaximized();

    //Save Settings
    connect(mMainWindow, &QMainWindow::destroyed, this, &Editor::saveSettings);

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
                                                        mSavePath,
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
                                                    mSavePath,
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
                                                    mSavePath,
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
                                                    mExportPath,
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

void Editor::loadSettings()
{
    QSettings settings;

    //Loading for meshViewDock
    QDockWidget *mvd = qobject_cast<QDockWidget *>(mMeshViewContainer->parentWidget());
    mvd->setHidden(settings.value("meshViewDock/hidden", false).toBool());
    mvd->setFloating(settings.value("meshViewDock/floating", false).toBool());
    mvd->move(settings.value("meshViewDock/pos").toPoint());
    mvd->resize(settings.value("meshViewDock/size").toSize());
    if(!settings.value("meshViewDock/floating").toBool(), false){
        mMainWindow->addDockWidget(
                    (Qt::DockWidgetArea)settings.value("meshViewDock/dockArea", Qt::RightDockWidgetArea).toInt()
                    , mvd);
    }

    //Loading for templateDock
    QDockWidget *td = qobject_cast<QDockWidget *>(mTileTemplateSetsView->parentWidget());
    td->setHidden(settings.value("templateDock/hidden", false).toBool());
    td->setFloating(settings.value("templateDock/floating", false).toBool());
    td->move(settings.value("templateDock/pos").toPoint());
    td->resize(settings.value("templateDock/size").toSize());
    if(!settings.value("templateDock/floating", false).toBool()){
        mMainWindow->addDockWidget(
                    (Qt::DockWidgetArea)settings.value("templateDock/dockArea", Qt::LeftDockWidgetArea).toInt()
                    , td);
    }

    //Loading for propBrowserDock
    QDockWidget *pbd = qobject_cast<QDockWidget *>(mPropertyBrowser->parentWidget());
    pbd->setHidden(settings.value("propBrowserDock/hidden", false).toBool());
    pbd->setFloating(settings.value("propBrowserDock/floating", false).toBool());
    pbd->move(settings.value("propBrowserDock/pos").toPoint());
    pbd->resize(settings.value("propBrowserDock/size").toSize());
    if(!settings.value("propBrowserDock/floating", false).toBool()){
        mMainWindow->addDockWidget(
                    (Qt::DockWidgetArea)settings.value("propBrowserDock/dockArea", Qt::RightDockWidgetArea).toInt()
                    , pbd);
    }

    //Loading for materialDock
    QDockWidget *md = qobject_cast<QDockWidget *>(mMaterialView->parentWidget());
    md->setHidden(settings.value("materialDock/hidden", false).toBool());
    md->setFloating(settings.value("materialDock/floating", false).toBool());
    md->move(settings.value("materialDock/pos").toPoint());
    md->resize(settings.value("materialDock/size").toSize());
    if(!settings.value("materialDock/floating", false).toBool()){
        mMainWindow->addDockWidget(
                    (Qt::DockWidgetArea)settings.value("materialDock/dockArea", Qt::RightDockWidgetArea).toInt()
                    , md);
    }

    //Loading saving and export paths
    mSavePath = settings.value("savePath", QString("/home/")).toString();
    mExportPath = settings.value("exportPath", QString("/home/")).toString();

}

void Editor::saveSettings()
{
    QSettings settings;

    //MeshViewDock settings
    QDockWidget *mvd = qobject_cast<QDockWidget *>(mMeshViewContainer->parentWidget());
    settings.beginGroup("meshViewDock");
    settings.setValue("pos", mvd->pos());
    settings.setValue("floating", mvd->isFloating());
    settings.setValue("hidden", mvd->isHidden());
    settings.setValue("size", mvd->size());
    if(mMainWindow->isVisible()){
        int i = mMainWindow->dockWidgetArea(mvd);
        settings.setValue("dockArea", i);
    }
    settings.endGroup();

    //templateDock settings
    QDockWidget *td = qobject_cast<QDockWidget *>(mTileTemplateSetsView->parentWidget());
    settings.beginGroup("templateDock");
    settings.setValue("pos", td->pos());
    settings.setValue("floating", td->isFloating());
    settings.setValue("hidden", td->isHidden());
    settings.setValue("size", td->size());
    if(mMainWindow->isVisible()){
        int i = mMainWindow->dockWidgetArea(td);
        settings.setValue("dockArea", i);
    }
    settings.endGroup();

    //propBrowserDock settings
    QDockWidget *pbd = qobject_cast<QDockWidget *>(mPropertyBrowser->parentWidget());
    settings.beginGroup("propBrowserDock");
    settings.setValue("pos", pbd->pos());
    settings.setValue("floating", pbd->isFloating());
    settings.setValue("hidden", pbd->isHidden());
    settings.setValue("size", pbd->size());
    if(mMainWindow->isVisible()){
        int i = mMainWindow->dockWidgetArea(pbd);
        settings.setValue("dockArea", i);
    }
    settings.endGroup();

    //materialDock settings
    QDockWidget *md = qobject_cast<QDockWidget *>(mMaterialView->parentWidget());
    settings.beginGroup("materialDock");
    settings.setValue("pos", md->pos());
    settings.setValue("floating", md->isFloating());
    settings.setValue("hidden", md->isHidden());
    settings.setValue("size", md->size());
    if(mMainWindow->isVisible()){
        int i = mMainWindow->dockWidgetArea(md);
        settings.setValue("dockArea", i);
    }
    settings.endGroup();

    //Save and Export paths
    settings.setValue("savePath", mSavePath);
    settings.setValue("exportPath", mExportPath);

}
