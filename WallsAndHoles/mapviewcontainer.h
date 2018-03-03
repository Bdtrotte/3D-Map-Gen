#ifndef MAPVIEWCONTAINER_H
#define MAPVIEWCONTAINER_H

#include "mapview.h"

#include <QWidget>

class MapViewContainer : public QWidget
{
    Q_OBJECT

public:
    MapViewContainer(QWidget *parent);

    MapView *mapView() { return mMapView; }

private:
    MapView *mMapView;
};

#endif // MAPVIEWCONTAINER_H
