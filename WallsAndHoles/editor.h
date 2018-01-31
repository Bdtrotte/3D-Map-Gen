#ifndef EDITOR_H
#define EDITOR_H

#include "mainwindow.h"
#include "tilemap.h"
#include "tilemaptoolmanager.h"
#include "mapview.h"
#include "map2mesh.h"
#include "meshviewcontainer.h"

#include <QObject>
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
    void createNewMap();

    void updateScene();

private:
    MainWindow *mMainWindow;

    // Map-to-Mesh Converter
    Map2Mesh *mMap2Mesh;

    //TileMap data
    TileMap *mTileMap;
    QRegion mTileMapSelectedRegion;

    //views
    MapView *mMapView;
    MeshViewContainer *mMeshViewContainer;

    //Tools
    TileMapToolManager *mTileMapToolManager;
    QToolBar *mToolBar;
};

#endif // EDITOR_H
