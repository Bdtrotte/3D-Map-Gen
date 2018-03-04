#include "mapviewcontainer.h"

#include <QToolBar>
#include <QVBoxLayout>
#include <QDebug>

MapViewContainer::MapViewContainer(QWidget *parent)
    : QWidget(parent)
    , mMapView(new MapView(this))
{
    QToolBar *toolBar = new QToolBar(this);
    mNoViewAction = toolBar->addAction("No View");
    mMapModeActions += QPair<QAction *, MapViewMode>(toolBar->addAction("Default View"), DefaultView);
    mMapModeActions += QPair<QAction *, MapViewMode>(toolBar->addAction("Height View"), HeightMapView);

    mNoViewAction->setCheckable(true);
    for (QPair<QAction *, MapViewMode> m : mMapModeActions)
        m.first->setCheckable(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(toolBar);
    layout->addWidget(mMapView);

    setLayout(layout);

    connect(mNoViewAction, &QAction::toggled,
            this, &MapViewContainer::noViewToggled);
    for (QPair<QAction *, MapViewMode> m : mMapModeActions) {
        connect(m.first, &QAction::toggled,
                this, &MapViewContainer::modeChanged);
    }

    mMapModeActions[0].first->setChecked(true);
}

void MapViewContainer::modeChanged()
{
    int viewMode = 0;
    for (QPair<QAction *, MapViewMode> m : mMapModeActions)
        if (m.first->isChecked())
            viewMode |= m.second;

    if (viewMode == 0) {
        mNoViewAction->setChecked(true);
        return;
    }

    mMapView->setViewMode(viewMode);

    if (mNoViewAction->isChecked()) {
        mNoViewAction->blockSignals(true);
        mNoViewAction->setChecked(false);
        mNoViewAction->blockSignals(false);
    }
}

void MapViewContainer::noViewToggled(bool state)
{
    if (state) {
        for (QPair<QAction *, MapViewMode> m : mMapModeActions) {
            m.first->blockSignals(true);
            m.first->setChecked(false);
            m.first->blockSignals(false);
        }

        mMapView->setViewMode(NoView);
    } else if (!mMapModeActions.isEmpty()) {
        mMapModeActions[0].first->setChecked(true);
    } else {
        mNoViewAction->setChecked(true);
    }
}
