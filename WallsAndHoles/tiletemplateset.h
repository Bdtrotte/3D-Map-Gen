#ifndef TILESET_H
#define TILESET_H

#include "tiletemplate.h"

#include <QObject>
#include <QVector>

class TileTemplateSet : public QObject
{
    Q_OBJECT

public:
    explicit TileTemplateSet(QObject *parent = nullptr);

    //Adds the given tileTemplate to the end of the tileList
    //should pass new TileTemplate(...) to this
    void addTileTemplate(SharedTileTemplate tileTemplate);

    //removes the tiletemplate at the specified index
    void removeTileTemplate(int index);

    const QVector<SharedTileTemplate> &cTileTemplates() const { return mTileTemplates; }
    QVector<SharedTileTemplate> &tileTemplates() { return mTileTemplates; }

signals:
    void tileTemplateAdded(int tileId);
    void tileTemplateRemoved(int tileId);

public slots:

private:
    QVector<SharedTileTemplate> mTileTemplates;
};

#endif // TILESET_H
