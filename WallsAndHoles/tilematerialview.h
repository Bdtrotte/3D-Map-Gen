#ifndef TILEMATERIALVIEW_H
#define TILEMATERIALVIEW_H

#include "tilematerialset.h"

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

private:
    TileMaterialSet *mTileMaterialSet;

    QListView *mMaterialList;
};

#endif // TILEMATERIALVIEW_H
