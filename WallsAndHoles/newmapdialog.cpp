#include "newmapdialog.h"

NewMapDialog::NewMapDialog(QDialog *parent)
  : QDialog(parent),
  result(NewMapData(-1,-1))
{
    //Setup the dialog
    QDialog::resize(400,200);
    QDialog::setWindowTitle("New Map");
    QDialog::setModal(true);

    dialogBtnBox = new QDialogButtonBox(this);
    dialogBtnBox->addButton("OK",QDialogButtonBox::AcceptRole);
    dialogBtnBox->addButton("Cancel",QDialogButtonBox::RejectRole);

    setupSpinBox();
    setupCheckBox();
    setMainLayout();

    connect(dialogBtnBox, &QDialogButtonBox::accepted, this, &NewMapDialog::setResult);
    connect(dialogBtnBox, &QDialogButtonBox::rejected, this, &NewMapDialog::cancelled);

    connect(outdoorMapCB, &QCheckBox::toggled, this,
            [this](bool checked){TileMapProperty.outdoorMap = checked;});
    connect(indoorMapCB, &QCheckBox::toggled, this,
            [this](bool checked){TileMapProperty.indoorMap = checked;});
    connect(ceilingCB, &QCheckBox::toggled, this,
            [this](bool checked){TileMapProperty.ceiling = checked;});

}

void NewMapDialog::setResult(){
    result = NewMapData(wSpinBox->value(), hSpinBox->value());
    accepted = true;
    this->close();
}

void NewMapDialog::cancelled(){
    accepted = false;
    this->close();
}

void NewMapDialog::setupSpinBox(){
    //Setup the spinbox
    heightLabel = new QLabel(this);
    widthLabel = new QLabel(this);
    heightLabel->setText("Height:");
    widthLabel->setText("Width:");

    hSpinBox = new QSpinBox(this);
    wSpinBox = new QSpinBox(this);
    hSpinBox->setMinimum(1);
    wSpinBox->setMinimum(1);
}

void NewMapDialog::setupCheckBox(){
    //Setup the properties checkboxes
    propertiesGroupBox = new QGroupBox(tr("Properties"));

    outdoorMapCB = new QCheckBox(this);
    indoorMapCB = new QCheckBox(this);
    ceilingCB = new QCheckBox(this);
    outdoorMapCB->setText("Outdoor Map");
    indoorMapCB->setText("Indoor Map");
    ceilingCB->setText("Ceiling");

    outdoorMapCB->setChecked(true);
    outdoorMapCB->setAutoExclusive(true);
    indoorMapCB->setAutoExclusive(true);

    checkboxLayout = new QVBoxLayout;
    checkboxLayout->addWidget(outdoorMapCB);
    checkboxLayout->addWidget(indoorMapCB);
    checkboxLayout->addWidget(ceilingCB);

    propertiesGroupBox->setLayout(checkboxLayout);
}

void NewMapDialog::setMainLayout(){
    //Sets the layout of the window
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(widthLabel, 0, 0);
    mainLayout->addWidget(wSpinBox, 0, 1);
    mainLayout->addWidget(heightLabel, 0, 2);
    mainLayout->addWidget(hSpinBox, 0, 3);
    mainLayout->addWidget(propertiesGroupBox, 1, 0, 2, 0);
    mainLayout->addWidget(dialogBtnBox, 4, 0, 1, 0);
    mainLayout->setMargin(20);
    this->setLayout(mainLayout);
}
