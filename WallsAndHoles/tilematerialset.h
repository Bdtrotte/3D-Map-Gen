#ifndef TILEMATERIALSET_H
#define TILEMATERIALSET_H

#include "tilematerial.h"

#include <QAbstractItemModel>

class TileMaterialSet : public QAbstractItemModel
{
    Q_OBJECT

public:
    TileMaterialSet(QObject *parent = nullptr);

    void addMaterial(TileMaterial *material);
    void removeMaterial(int index);

    TileMaterial *materialAt(int index);
    const TileMaterial *materialAt(int index) const;

    int size() const { return mMaterials.size(); }

    //Model Functions:
    QModelIndex index(int row, int, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<TileMaterial *> mMaterials;
};

#endif // TILEMATERIALSET_H
