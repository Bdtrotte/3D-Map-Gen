#include "newmapdialogs.h"

NewMapDialogs::NewMapDialogs(QWidget *parent)
  : QWidget(parent),
  result(NewMapDatas(-1,-1))
{
    QWidget::setWindowTitle("New Map");
    QWidget::resize(QSize(400,200));

    QDialogButtonBox *dialogBtnBox = new QDialogButtonBox(this);
    dialogBtnBox->addButton("OK",QDialogButtonBox::AcceptRole);
    dialogBtnBox->addButton("Cancel",QDialogButtonBox::RejectRole);

    QLabel *heightLabel = new QLabel(this);
    QLabel *widthLabel = new QLabel(this);
    heightLabel->setText("Height:");
    widthLabel->setText("Width:");

    QSpinBox *hSpinBox = new QSpinBox(this);
    QSpinBox *wSpinBox = new QSpinBox(this);
    hSpinBox->setMinimum(1);
    wSpinBox->setMinimum(1);

    QLabel *prop = new QLabel(this);
    prop->setText("Properties");

    QCheckBox *outdoorMapCB = new QCheckBox(this);
    QCheckBox *indoorMapCB = new QCheckBox(this);
    QCheckBox *ceilingCB = new QCheckBox(this);
    outdoorMapCB->setText("Outdoor Map");
    indoorMapCB->setText("Indoor Map");
    ceilingCB->setText("Ceiling");

    QVBoxLayout *checkboxLayout = new QVBoxLayout;
    checkboxLayout->addWidget(prop);
    checkboxLayout->addWidget(outdoorMapCB);
    checkboxLayout->addWidget(indoorMapCB);
    checkboxLayout->addWidget(ceilingCB);


    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(widthLabel, 0, 0);
    mainLayout->addWidget(wSpinBox, 0, 1);
    mainLayout->addWidget(heightLabel, 0, 2);
    mainLayout->addWidget(hSpinBox, 0, 3);
    mainLayout->addLayout(checkboxLayout, 1, 0, 2, 0);
    mainLayout->addWidget(dialogBtnBox, 4, 0, 1, 0);
    mainLayout->setMargin(20);

    this->setLayout(mainLayout);
    this->show();
}
