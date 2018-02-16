#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include "tilemap.h"

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>

struct NewMapData
{
    int width;
    int height;
    bool isIndoorMap;
    bool hasCeiling;

    NewMapData() {}

    NewMapData(int w, int h, bool indoor, bool ceiling)
        : width(w)
        , height(h)
        , isIndoorMap(indoor)
        , hasCeiling(ceiling) {}
};

class NewMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewMapDialog(QDialog *parent = nullptr);

    NewMapData result;

private slots:
    void onAccepted();

private:
    //Spin Box
    QSpinBox *mHSpinBox;
    QSpinBox *mWSpinBox;

    //CheckBox
    QCheckBox *mIndoorMapChkBox;
    QCheckBox *mCeilingChkBox;
};

#endif // NEWMAPDIALOG_H
