#ifndef EDITOR_H
#define EDITOR_H

#include "mainwindow.h"
#include "tilemap.h"
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

public slots:
    void createNewMap();

private:
    MainWindow *mMainWindow;
    MapView *mMapView;
    QToolBar *mToolBar;
};

#endif // EDITOR_H
