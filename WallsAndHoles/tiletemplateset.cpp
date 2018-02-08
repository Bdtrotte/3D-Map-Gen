#include "tiletemplateset.h"

#include "xmltool.h"

TileTemplateSet::TileTemplateSet(QString savePath,
                                 QString name,
                                 bool loadedFromFile,
                                 QObject *parent)
    : QAbstractItemModel(parent)
    , mName(name)
    , mSavePath(savePath)
    , mSaved(loadedFromFile) {}

void TileTemplateSet::addTileTemplate(SharedTileTemplate tileTemplate,  bool dontAffectSaveStatus)
{
    if (!dontAffectSaveStatus)
        changed();

    beginInsertRows(QModelIndex(), size(), size());
    mTileTemplates.append(tileTemplate);
    endInsertRows();

    connect(tileTemplate.data(), &TileTemplate::changed,
            this, &TileTemplateSet::templateChanged);
}

void TileTemplateSet::removeTileTemplate(int index)
{
    Q_ASSERT(index >= 0 && index < mTileTemplates.size());

    changed();

    beginRemoveRows(QModelIndex(), index, index);
    mTileTemplates.removeAt(index);
    endRemoveRows();
}

void TileTemplateSet::save()
{
    // TODO be loader when saving fails
    if (XMLTool::saveTileTemplateSet(this) != 0)
        return;

    mSaved = true;
    emit saveStateChanged(mSaved);
}

QModelIndex TileTemplateSet::index(int row, int, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();

    return createIndex(row, 0);
}

QModelIndex TileTemplateSet::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int TileTemplateSet::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return size();

    return 0;
}

int TileTemplateSet::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
}

QVariant TileTemplateSet::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (!index.parent().isValid()) {
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return mTileTemplates[index.row()]->name();
        case Qt::DecorationRole:
            return mTileTemplates[index.row()]->color();
        case Qt::ToolTipRole:
            return tr("A Tile Template");
        }
    }

    return QVariant();
}

bool TileTemplateSet::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        switch (role) {
        case Qt::EditRole:
            mTileTemplates[index.row()]->setName(value.toString());
        default:
            return false;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TileTemplateSet::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.parent().isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable
            | Qt::ItemIsSelectable
            | Qt::ItemIsEnabled;
}

void TileTemplateSet::changed()
{
    if (mSaved) {
        emit saveStateChanged(false);
    }

    mSaved = false;
}
