#ifndef TILEMATERIALVIEW_H
#define TILEMATERIALVIEW_H

#include "tilematerialset.h"
#include "propertybrowser.h"

#include <QWidget>
#include <QListView>

class TileMaterialView : public QWidget
{
    Q_OBJECT

public:
    explicit TileMaterialView(QWidget *parent = nullptr);

private slots:
    void addMaterial();
    void removeMaterial();

    void materialSelected();

private:
    TileMaterialSet *mTileMaterialSet;

    QListView *mMaterialList;

    PropertyBrowser *mPropertyBrowser;
};

#endif // TILEMATERIALVIEW_H
