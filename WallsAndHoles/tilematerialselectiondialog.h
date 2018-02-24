#ifndef TILEMATERIALSELECTIONDIALOG_H
#define TILEMATERIALSELECTIONDIALOG_H

#include <QDialog>
#include <QListView>

class TileMaterialSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TileMaterialSelectionDialog(QWidget *parent = nullptr);

    int result = 0; //default material

private slots:
    void selectedChanged();

private:
    QListView *mMaterialList;
};

#endif // TILEMATERIALSELECTIONDIALOG_H
