#include "newmapdialog.h"
#include "ui_newmapdialog.h"

#include <QMessageBox>

NewMapDialog::NewMapDialog(QWidget *parent) :
    QDialog(parent),
    result(NewMapData(-1, -1)),
    ui(new Ui::NewMapDialog())
{
    ui->setupUi(this);

    oneChecked = ui->checkBox_Ceiling->isChecked()||ui->checkBox_Default->isChecked()||
            ui->checkBox_Indoor->isChecked()||ui->checkBox_Outdoor->isChecked();
}

NewMapDialog::~NewMapDialog()
{
    delete ui;
}

void NewMapDialog::on_buttonBox_accepted()
{
    result = NewMapData(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}

void NewMapDialog::on_checkBox_Default_toggled(bool checked)
{
    if(checked)
        TileMapProperty.defaultMap = true;
    else
        TileMapProperty.defaultMap = false;
}

void NewMapDialog::on_checkBox_Outdoor_toggled(bool checked)
{
    if(checked)
        TileMapProperty.outsideMap = true;
    else
        TileMapProperty.outsideMap = false;
}

void NewMapDialog::on_checkBox_Indoor_toggled(bool checked)
{
    if(checked)
        TileMapProperty.indoorMap = true;
    else
        TileMapProperty.indoorMap = false;
}

void NewMapDialog::on_checkBox_Ceiling_toggled(bool checked)
{
    if(checked)
        TileMapProperty.ceiling = true;
    else
        TileMapProperty.ceiling = false;
}
