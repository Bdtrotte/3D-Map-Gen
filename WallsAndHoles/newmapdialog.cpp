#include "newmapdialog.h"
#include "ui_newmapdialog.h"

NewMapDialog::NewMapDialog(QWidget *parent) :
    QDialog(parent),
    result(NewMapData(-1, -1)),
    ui(new Ui::NewMapDialog())
{
    ui->setupUi(this);
    ui->noPropertyRadioBtn->setChecked(true);
}

NewMapDialog::~NewMapDialog()
{
    delete ui;
}

void NewMapDialog::on_buttonBox_accepted()
{
    result = NewMapData(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}


void NewMapDialog::on_noPropertyRadioBtn_clicked()
{
    TileMapProperty = NoProperty;
}
void NewMapDialog::on_outdoorRadioBtn_clicked()
{
    TileMapProperty = OutdoorMap;
}

void NewMapDialog::on_indoorRadioBtn_clicked()
{
    TileMapProperty = IndoorMap;
}
