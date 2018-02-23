#include "tilematerialselectiondialog.h"

#include "tilematerialset.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>

TileMaterialSelectionDialog::TileMaterialSelectionDialog(QWidget *parent)
    : QDialog(parent)
    , mMaterialList(new QListView(this))
{
    mMaterialList->setModel(TileMaterialSet::getInstance());

    //Buttons
    QDialogButtonBox *dialogBtnBox = new QDialogButtonBox(this);
    dialogBtnBox->addButton("OK", QDialogButtonBox::AcceptRole);
    dialogBtnBox->addButton("Cancel", QDialogButtonBox::RejectRole);

    //Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(mMaterialList);
    layout->addWidget(dialogBtnBox);

    setLayout(layout);

    connect(mMaterialList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &TileMaterialSelectionDialog::selectedChanged);

    connect(dialogBtnBox, &QDialogButtonBox::accepted,
            this, &TileMaterialSelectionDialog::accept);
    connect(dialogBtnBox, &QDialogButtonBox::rejected,
            this, &TileMaterialSelectionDialog::reject);
}

void TileMaterialSelectionDialog::selectedChanged()
{
    result = mMaterialList->selectionModel()->currentIndex().row();
}
