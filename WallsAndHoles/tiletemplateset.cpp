#include "tiletemplateset.h"

#include "xmltool.h"

TileTemplateSet::TileTemplateSet(QString name,
                                 QObject *parent)
    : QAbstractItemModel(parent)
    , mName(name) {}

void TileTemplateSet::addTileTemplate(TileTemplate *tileTemplate,  bool dontAffectSaveStatus)
{
    if (tileTemplate)
        tileTemplate->setParent(this);

    if (!dontAffectSaveStatus)
        changed();

    beginInsertRows(QModelIndex(), size(), size());
    mTileTemplates.append(tileTemplate);
    endInsertRows();

    if (tileTemplate) {
        connect(tileTemplate, &TileTemplate::changed,
                this, &TileTemplateSet::templateChanged);
    }
}

void TileTemplateSet::removeTileTemplate(int index)
{
    Q_ASSERT(index >= 0 && index < mTileTemplates.size());

    changed();

    beginRemoveRows(QModelIndex(), index, index);
    mTileTemplates.removeAt(index);
    endRemoveRows();
}

QModelIndex TileTemplateSet::index(int row, int, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row >= mTileTemplates.size())
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
            if (TileTemplate *t = mTileTemplates[index.row()])
                return t->name();
            else
                return "Ground (Eraser)";
        case Qt::DecorationRole:
            if (TileTemplate *t = mTileTemplates[index.row()])
                return t->color();
            else
                return QColor(0,0,0);
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
            if (TileTemplate *t = mTileTemplates[index.row()])
                t->setName(value.toString());
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

    if (mTileTemplates[index.row()]) {
        return Qt::ItemIsEditable
                | Qt::ItemIsSelectable
                | Qt::ItemIsEnabled;
    } else {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
}
