#ifndef TILESET_H
#define TILESET_H

#include "tiletemplate.h"

#include <QAbstractItemModel>
#include <QList>

/**
 * @brief The TileTemplateSet class
 * Stores a list of tile templates. Also implements the item model
 * for them.
 */
class TileTemplateSet : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TileTemplateSet(QString name = "New Tile Template Set",
                             QObject *parent = nullptr);

    //General Items::

    //Adds the given tileTemplate to the end of the tileList
    //should pass new TileTemplate(...) to this
    void addTileTemplate(TileTemplate *tileTemplate, bool dontAffectSaveStatus = false);

    //removes the tiletemplate at the specified index
    void removeTileTemplate(int index);

    TileTemplate *tileTemplateAt(int i) { return mTileTemplates[i]; }
    const TileTemplate *cTileTemplateAt(int i) const { return mTileTemplates[i]; }

    QString name() const { return mName; }
    void setName(QString name) { changed(); mName = name; }

    int size() const { return mTileTemplates.size(); }

    const QList<TileTemplate *> &cTileTemplates() const { return mTileTemplates; }

    //Model Functions::
    QModelIndex index(int row, int,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private slots:
    void templateChanged() { changed(); }

protected:
    //should be called whenever data needing to be saved changes
    //To be used in sub classes
    virtual void changed() {}

private:
    QString mName;

    QList<TileTemplate *> mTileTemplates;
};

#endif // TILESET_H
