#include "tilematerialset.h"

#include <QPixmap>
#include <QMimeData>

TileMaterialSet::TileMaterialSet()
    : QAbstractItemModel() {}

TileMaterialSet *TileMaterialSet::tileMaterialSet = nullptr;

TileMaterialSet *TileMaterialSet::getInstance()
{
    if (!tileMaterialSet)
        tileMaterialSet = new TileMaterialSet();
    return tileMaterialSet;
}

void TileMaterialSet::addMaterial(TileMaterial *material)
{
    Q_ASSERT(mMaterials.indexOf(material) == -1);

    material->setParent(this);

    beginInsertRows(QModelIndex(), size() + 1, size() + 1);
    mMaterials.append(material);
    endInsertRows();
}

void TileMaterialSet::removeMaterial(int index)
{
    //can't remove the default
    Q_ASSERT(index > 0 && index <= size());

    // TODO: Needs to handle anything currently using this material

    beginRemoveRows(QModelIndex(), index, index);
    TileMaterial *tm = mMaterials.takeAt(index - 1);
    endRemoveRows();

    delete tm;
}

TileMaterial *TileMaterialSet::materialAt(int index)
{
    if (index == 0)
        return TileMaterial::getDefaultMaterial();
    else
        return mMaterials[index - 1];
}

const TileMaterial *TileMaterialSet::materialAt(int index) const
{
    if (index == 0)
        return TileMaterial::getDefaultMaterial();
    else
        return mMaterials[index - 1];
}

QModelIndex TileMaterialSet::index(int row, int, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row > size())
        return QModelIndex();

    return createIndex(row, 0);
}

QModelIndex TileMaterialSet::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int TileMaterialSet::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return size() + 1;

    return 0;
}

int TileMaterialSet::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
}

QVariant TileMaterialSet::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.parent().isValid())
        return QVariant();

    TileMaterial *m;
    if (index.row() == 0)
        m = TileMaterial::getDefaultMaterial();
    else
        m = mMaterials[index.row() - 1];

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return m->name();
    case Qt::DecorationRole:
        if (m->texture().isNull())
            return QVariant();
        else
            return QPixmap::fromImage(*m->texture()->image()).scaled(20, 20);
    }

    return QVariant();
}

bool TileMaterialSet::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) == value || index.row() == 0) return false;

    switch (role) {
    case Qt::EditRole:
        mMaterials[index.row() - 1]->setName(value.toString());
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
}

QMimeData *TileMaterialSet::mimeData(const QModelIndexList &indexes) const
{
    //Can't handle more than one for now.
    Q_ASSERT(indexes.size() == 1);

    QMimeData *mimeData = new QMimeData();

    mimeData->setText("TileMaterial: " + QString::number(indexes[0].row()));

    return mimeData;
}

Qt::ItemFlags TileMaterialSet::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.parent().isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    if (index.row() > 0)
        f |= Qt::ItemIsEditable;

    return f;
}
