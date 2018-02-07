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

private slots:
    void on_buttonBox_accepted();

    void on_outdoorRadioBtn_clicked();

    void on_noPropertyRadioBtn_clicked();

    void on_indoorRadioBtn_clicked();

private:
    Ui::NewMapDialog *ui;
};

#endif // NEWMAPDIALOG_H
