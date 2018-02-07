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
    explicit TileTemplateSet(QString savePath,
                             QString name = "New Tile Template Set",
                             bool loadedFromFile = false,
                             QObject *parent = nullptr);

    //General Items::

    //Adds the given tileTemplate to the end of the tileList
    //should pass new TileTemplate(...) to this
    void addTileTemplate(SharedTileTemplate tileTemplate, bool dontAffectSaveStatus = false);

    //removes the tiletemplate at the specified index
    void removeTileTemplate(int index);

    SharedTileTemplate tileTemplateAt(int i) { return mTileTemplates[i]; }
    const SharedTileTemplate &cTileTemplateAt(int i) const { return mTileTemplates[i]; }

    QString name() const { return mName; }
    void setName(QString name) { changed(); mName = name; }

    int size() const { return mTileTemplates.size(); }

    const QList<SharedTileTemplate> &cTileTemplates() const { return mTileTemplates; }

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }

    void save();
    bool isSaved() const { return mSaved; }

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

    const QString savePath() const { return mSavePath; }
    void setSavePath(QString path){ mSavePath = path; }

signals:
    void saveStateChanged(bool state);

private slots:
    void templateChanged() { changed(); }

private:
    //should be called whenever data needing to be saved changes
    void changed();

    QString mName;

    QList<SharedTileTemplate> mTileTemplates;

    //default save path of this tileTempalteSet object
    QString mSavePath;

    //whether or not the current state of this is saved. (made false when this is changed)
    bool mSaved;
};

typedef QSharedPointer<TileTemplateSet> SharedTileTemplateSet;
#endif // TILESET_H
