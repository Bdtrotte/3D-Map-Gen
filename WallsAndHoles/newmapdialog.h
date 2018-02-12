#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include "tilemap.h"

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>

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
    explicit NewMapDialog(QDialog *parent = nullptr);

    NewMapData result;

    Properties TileMapProperty;

    bool accepted;

private:
    QDialogButtonBox *dialogBtnBox;

    //Spin Box
    QSpinBox *hSpinBox;
    QSpinBox *wSpinBox;
    QLabel *heightLabel;
    QLabel *widthLabel;

    //CheckBox
    QCheckBox *outdoorMapCB;
    QCheckBox *indoorMapCB;
    QCheckBox *ceilingCB;
    QGroupBox *propertiesGroupBox;
    QVBoxLayout *checkboxLayout;

    //Main layout
    QGridLayout *mainLayout;

    void setupSpinBox();
    void setupCheckBox();
    void setMainLayout();
signals:

protected slots:
    void setResult();
    void cancelled();
};

#endif // NEWMAPDIALOG_H
