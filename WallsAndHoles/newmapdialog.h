#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QAbstractButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

struct NewMapData
{
    int width;
    int height;

    NewMapData(int w, int h) : width(w), height(h) {}
};

class NewMapDialog : public QWidget
{
    Q_OBJECT
public:
    explicit NewMapDialog(QWidget *parent = nullptr);

    NewMapData result;

private:
    QSpinBox *hSpinBox;
    QSpinBox *wSpinBox;

signals:

public slots:
protected slots:
    void setResult();
};

#endif // NEWMAPDIALOG_H
