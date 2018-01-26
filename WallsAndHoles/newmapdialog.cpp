#include "newmapdialog.h"
#include "ui_newmapdialog.h"

NewMapDialog::NewMapDialog(QWidget *parent) :
    QDialog(parent),
    result(NewMapData(-1, -1)),
    ui(new Ui::NewMapDialog())
{
    ui->setupUi(this);
}

NewMapDialog::~NewMapDialog()
{
    delete ui;
}

void NewMapDialog::on_buttonBox_accepted()
{
    result = NewMapData(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}
