#ifndef EDITOR_H
#define EDITOR_H

#include "tilemap.h"
#include "tilemaptoolmanager.h"
#include "mapview.h"

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
    void newMap();
    void saveMap();
    void loadMap();
    void exportMapMesh();

private:
    void setUpMenuBar();

    QMainWindow *mMainWindow;

    //TileMap data
    TileMap *mTileMap;
    QRegion mTileMapSelectedRegion;

    //views
    MapView *mMapView;

    //Tools
    TileMapToolManager *mTileMapToolManager;
    QToolBar *mToolBar;
};

#endif // EDITOR_H
