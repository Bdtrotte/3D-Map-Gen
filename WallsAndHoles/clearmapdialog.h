#ifndef CLEARMAPDIALOG_H
#define CLEARMAPDIALOG_H

#include <QDialog>

namespace Ui {
class ClearMapDialog;
}

class ClearMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClearMapDialog(QWidget *parent = 0);
    bool accepted = false;
    ~ClearMapDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ClearMapDialog *ui;
};

#endif // CLEARMAPDIALOG_H
