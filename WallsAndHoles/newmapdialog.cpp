#include "newmapdialog.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>

NewMapDialog::NewMapDialog(QDialog *parent)
    : QDialog(parent)
{
    //Setup the dialog
    QDialog::resize(400,200);
    QDialog::setWindowTitle("New Map");
    QDialog::setModal(true);


    //SpinBox Setup
    QLabel *heightLabel = new QLabel("Height:", this);
    QLabel *widthLabel = new QLabel("Width:", this);

    mHSpinBox = new QSpinBox(this);
    mWSpinBox = new QSpinBox(this);
    mHSpinBox->setMinimum(1);
    mWSpinBox->setMinimum(1);
    mHSpinBox->setMaximum(1000);
    mWSpinBox->setMaximum(1000);


    //Properties setup
    QGroupBox *propertiesGroupBox = new QGroupBox(tr("Properties"));

    QCheckBox *outdoorMapChkBox = new QCheckBox("Outdoor Map", this);
    mIndoorMapChkBox = new QCheckBox("Indoor Map", this);
    mCeilingChkBox = new QCheckBox("Ceiling", this);

    outdoorMapChkBox->setChecked(true);
    outdoorMapChkBox->setAutoExclusive(true);
    mIndoorMapChkBox->setAutoExclusive(true);

    QVBoxLayout *checkboxLayout = new QVBoxLayout;
    checkboxLayout->addWidget(outdoorMapChkBox);
    checkboxLayout->addWidget(mIndoorMapChkBox);
    checkboxLayout->addWidget(mCeilingChkBox);

    propertiesGroupBox->setLayout(checkboxLayout);


    //Buttons
    QDialogButtonBox *dialogBtnBox = new QDialogButtonBox(this);
    dialogBtnBox->addButton("OK",QDialogButtonBox::AcceptRole);
    dialogBtnBox->addButton("Cancel",QDialogButtonBox::RejectRole);


    //Main Layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(widthLabel, 0, 0);
    mainLayout->addWidget(mWSpinBox, 0, 1);
    mainLayout->addWidget(heightLabel, 0, 2);
    mainLayout->addWidget(mHSpinBox, 0, 3);
    mainLayout->addWidget(propertiesGroupBox, 1, 0, 2, 0);
    mainLayout->addWidget(dialogBtnBox, 4, 0, 1, 0);
    mainLayout->setMargin(20);
    setLayout(mainLayout);

    //Connections for the OK and Cancel buttons
    connect(dialogBtnBox, &QDialogButtonBox::accepted, this, &NewMapDialog::onAccepted);
    connect(dialogBtnBox, &QDialogButtonBox::rejected, this, [this]{ done(0); });
}

void NewMapDialog::onAccepted()
{
    result = NewMapData(mWSpinBox->value(),
                        mHSpinBox->value(),
                        mIndoorMapChkBox->isChecked(),
                        mCeilingChkBox->isChecked());
    done(1);
}
