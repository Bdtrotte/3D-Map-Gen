#include "tilematerialview.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

TileMaterialView::TileMaterialView(QWidget *parent)
    : QWidget(parent)
    , mTileMaterialSet(new TileMaterialSet(this))
    , mMaterialList(new QListView(this))
{
    mMaterialList->setModel(mTileMaterialSet);
    mMaterialList->setDragDropMode(QAbstractItemView::DragOnly);
    mMaterialList->setDragEnabled(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mMaterialList);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QPushButton *addMaterial = new QPushButton("Add Material", this);
    QPushButton *removeMaterial = new QPushButton("Remove Material", this);
    hLayout->addWidget(addMaterial);
    hLayout->addWidget(removeMaterial);

    layout->addLayout(hLayout);

    setLayout(layout);

    connect(addMaterial, &QPushButton::clicked,
            this, &TileMaterialView::addMaterial);
    connect(removeMaterial, &QPushButton::clicked,
            this, &TileMaterialView::removeMaterial);
}

void TileMaterialView::addMaterial()
{
    mTileMaterialSet->addMaterial(new TileMaterial());
}

void TileMaterialView::removeMaterial()
{

}
