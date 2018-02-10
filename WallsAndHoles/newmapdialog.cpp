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

void NewMapDialog::on_checkBox_Default_stateChanged(int arg1)
{
    if(arg1 == 0)
        TileMapProperty.defaultMap = false;
    else if(arg1 == 2)
        TileMapProperty.defaultMap = true;
}

void NewMapDialog::on_checkBox_Outdoor_stateChanged(int arg1)
{
    if(arg1 == 0)
        TileMapProperty.outsideMap = false;
    else if(arg1 == 2)
        TileMapProperty.outsideMap = true;
}

void NewMapDialog::on_checkBox_Indoor_stateChanged(int arg1)
{
    if(arg1 == 0)
        TileMapProperty.indoorMap = false;
    else if(arg1 == 2)
        TileMapProperty.indoorMap = true;
}

void NewMapDialog::on_checkBox_Ceiling_stateChanged(int arg1)
{
    if(arg1 == 0)
        TileMapProperty.ceiling = false;
    else if(arg1 == 2)
        TileMapProperty.ceiling = true;
}
