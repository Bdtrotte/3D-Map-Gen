#include "mapviewcontainer.h"

#include <QToolBar>
#include <QVBoxLayout>

MapViewContainer::MapViewContainer(QWidget *parent)
    : QWidget(parent)
    , mMapView(new MapView(this))
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction("No View");
    toolBar->addAction("Default View");
    toolBar->addAction("Height View");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(toolBar);
    layout->addWidget(mMapView);

    setLayout(layout);
}
