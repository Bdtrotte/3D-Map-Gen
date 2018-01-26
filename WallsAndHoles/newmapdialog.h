#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QDialog>

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

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewMapDialog *ui;
};

#endif // NEWMAPDIALOG_H
