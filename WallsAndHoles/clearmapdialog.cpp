#include "clearmapdialog.h"
#include "ui_clearmapdialog.h"

ClearMapDialog::ClearMapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClearMapDialog)
{
    ui->setupUi(this);
}

ClearMapDialog::~ClearMapDialog()
{
    delete ui;
}

void ClearMapDialog::on_buttonBox_accepted()
{
    accepted = true;
}
