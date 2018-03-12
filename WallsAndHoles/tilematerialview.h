#ifndef TILEMATERIALVIEW_H
#define TILEMATERIALVIEW_H

#include "tilematerialset.h"
#include "propertybrowser.h"

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QSplitter>

class TileMaterialView : public QWidget
{
    Q_OBJECT

public:
    explicit TileMaterialView(QWidget *parent = nullptr);\

    void saveState();
    void restoreState();

private slots:
    void addMaterial();
    void removeMaterial();

    void materialSelected();

private:
    TileMaterialSet *mTileMaterialSet;

    QListView *mMaterialList;
    QPushButton *mRemoveMaterialButton;

    PropertyBrowser *mPropertyBrowser;

    QSplitter *mSplitter;
};

#endif // TILEMATERIALVIEW_H
