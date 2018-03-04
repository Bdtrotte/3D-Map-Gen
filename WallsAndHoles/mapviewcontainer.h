#ifndef MAPVIEWCONTAINER_H
#define MAPVIEWCONTAINER_H

#include "mapview.h"
#include "mapcellgraphicsitem.h"

#include <QWidget>
#include <QAction>

class MapViewContainer : public QWidget
{
    Q_OBJECT

public:
    MapViewContainer(QWidget *parent);

    MapView *mapView() { return mMapView; }

private slots:
    void modeChanged();
    void noViewToggled(bool state);

private:
    MapView *mMapView;

    QAction *mNoViewAction;
    QVector<QPair<QAction *, MapViewMode>> mMapModeActions;
};

#endif // MAPVIEWCONTAINER_H
