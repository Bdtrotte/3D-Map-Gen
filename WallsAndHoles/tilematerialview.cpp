#include "tilematerialview.h"

#include "materialpropertymanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

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
    QPushButton *addMaterial = new QPushButton(QIcon(":/icons/22x22/add.png"), "Add Material", this);
    mRemoveMaterialButton = new QPushButton(QIcon(":/icons/22x22/remove.png"), "Remove Material", this);
    mRemoveMaterialButton->setEnabled(false);
    hLayout->addWidget(addMaterial);
    hLayout->addWidget(mRemoveMaterialButton);

    layout->addLayout(hLayout);

    w->setLayout(layout);

    mSplitter = new QSplitter(this);
    mSplitter->setOrientation(Qt::Vertical);
    mSplitter->addWidget(w);
    mSplitter->addWidget(mPropertyBrowser);

    layout = new QVBoxLayout;
    layout->addWidget(mSplitter);

    setLayout(layout);

    connect(addMaterial, &QPushButton::clicked,
            this, &TileMaterialView::addMaterial);
    connect(mRemoveMaterialButton, &QPushButton::clicked,
            this, &TileMaterialView::removeMaterial);

    connect(mMaterialList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &TileMaterialView::materialSelected);
}

void TileMaterialView::saveState()
{
    QSettings settings;

    settings.setValue("tilematerialview/splitter", mSplitter->saveState());
}

void TileMaterialView::restoreState()
{
    QSettings settings;

    mSplitter->restoreState(settings.value("tilematerialview/splitter").toByteArray());
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
