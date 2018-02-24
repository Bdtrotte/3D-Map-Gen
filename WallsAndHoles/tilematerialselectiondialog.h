#ifndef TILEMATERIALSELECTIONDIALOG_H
#define TILEMATERIALSELECTIONDIALOG_H

#include <QDialog>
#include <QListView>

class TileMaterialSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TileMaterialSelectionDialog(QWidget *parent = nullptr);

    int result() const { return mResult; }

private slots:
    void selectedChanged();

private:
    int mResult = 0; //default material

    QListView *mMaterialList;
};

#endif // TILEMATERIALSELECTIONDIALOG_H
