#include "tilematerialview.h"

#include "materialpropertymanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

TileMaterialView::TileMaterialView(QWidget *parent)
    : QWidget(parent)
    , mTileMaterialSet(TileMaterialSet::getInstance())
    , mMaterialList(new QListView(this))
    , mPropertyBrowser(new PropertyBrowser(this))
{
    mMaterialList->setModel(mTileMaterialSet);
    mMaterialList->setDragDropMode(QAbstractItemView::DragOnly);
    mMaterialList->setDragEnabled(true);

    QWidget *w = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mMaterialList);

    QHBoxLayout *hLayout = new QHBoxLayout;
    QPushButton *addMaterial = new QPushButton("Add Material", this);
    mRemoveMaterialButton = new QPushButton("Remove Material", this);
    hLayout->addWidget(addMaterial);
    hLayout->addWidget(mRemoveMaterialButton);

    layout->addLayout(hLayout);

    w->setLayout(layout);

    QSplitter *s = new QSplitter(this);
    s->setOrientation(Qt::Vertical);
    s->addWidget(w);
    s->addWidget(mPropertyBrowser);

    layout = new QVBoxLayout;
    layout->addWidget(s);

    setLayout(layout);

    connect(addMaterial, &QPushButton::clicked,
            this, &TileMaterialView::addMaterial);
    connect(mRemoveMaterialButton, &QPushButton::clicked,
            this, &TileMaterialView::removeMaterial);

    connect(mMaterialList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &TileMaterialView::materialSelected);
}

void TileMaterialView::addMaterial()
{
    mTileMaterialSet->addMaterial(new TileMaterial());
}

void TileMaterialView::removeMaterial()
{
    int ind = mMaterialList->selectionModel()->currentIndex().row();

    mTileMaterialSet->removeMaterial(ind);
}

void TileMaterialView::materialSelected()
{
    int ind = mMaterialList->selectionModel()->currentIndex().row();

    if (ind < 1) {
        mPropertyBrowser->clear();
        mRemoveMaterialButton->setEnabled(false);
    } else {
        mPropertyBrowser->setPropertyManager(new MaterialPropertyManager(mTileMaterialSet->materialAt(ind)));
        mRemoveMaterialButton->setEnabled(true);
    }
}
