#ifndef EDITOR_H
#define EDITOR_H

#include "tilemap.h"
#include "tilemaptoolmanager.h"
#include "tiletemplateset.h"
#include "mapview.h"
#include "xmltool.h"
#include "map2mesh.h"
#include "meshviewcontainer.h"
#include "tiletemplatesetsview.h"
#include "tilepropertyview.h"
#include "tiletemplatesetsmanager.h"

#include <QObject>
#include <QList>
#include <QToolBar>

/**
 * @brief The Editor class
 * This is responsible for creating and managing all windows of the editor.
 * MainWindow will be created, and filled with dockable widgets.
 * This owns all views, and handles all comunication between front and backend
 */

class Editor : public QObject
{
    Q_OBJECT

public:
    explicit Editor(QObject *parent = nullptr);
    ~Editor();

public slots:
    void newMap();
    void saveMap();
    void saveMapAs();
    void loadMap();
    void closeMap();
    void exportMapMesh();

private:
    //sets mTileMap to tileMap, and updates everything accordingly
    void setTileMap(TileMap *tileMap);
    void setUpMenuBar();

    QMainWindow *mMainWindow;

    // Map-to-Mesh Converter
    Map2Mesh *mMap2Mesh;

    //TileMap data
    TileMap *mTileMap;
    TileTemplateSetsManager *mTileTemplateSetManager;
    QRegion mTileMapSelectedRegion;

    //views
    MapView *mMapView;
    MeshViewContainer *mMeshViewContainer;
    TileTemplateSetsView *mTileTemplateSetsView;
    TilePropertyView *mTilePropertyView;

    //Tools
    TileMapToolManager *mTileMapToolManager;
    QToolBar *mToolBar;

    //QActions which should be disabled when there is no map.
    QVector<QAction*> mMapDependantActions;
    void setMapDependantActionsEnabled(bool enabled)
    {
        for (QAction *a : mMapDependantActions)
            a->setEnabled(enabled);
    }
};

#endif // EDITOR_H
