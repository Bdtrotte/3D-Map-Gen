#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QDialog>
#include "tilemap.h"

namespace Ui {
class NewMapDialog;
}

struct NewMapData
{
    int width;
    int height;

    NewMapData(int w, int h) : width(w), height(h) {}
};

class NewMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewMapDialog(QWidget *parent = 0);
    ~NewMapDialog();

    NewMapData result;
    Properties TileMapProperty;
    //Checks if at least one property is selected
    bool oneChecked;

private slots:
    void on_buttonBox_accepted();

    void on_checkBox_Default_toggled(bool checked);

    void on_checkBox_Outdoor_toggled(bool checked);

    void on_checkBox_Indoor_toggled(bool checked);

    void on_checkBox_Ceiling_toggled(bool checked);

private:
    Ui::NewMapDialog *ui;
};

#endif // NEWMAPDIALOG_H
