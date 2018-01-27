#ifndef TILESET_H
#define TILESET_H

#include "tiletemplate.h"

#include <QObject>
#include <QList>

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

    //get a const referance to tileTemplate of a specified tileId
    const SharedTileTemplate getCTileTemplate(int tileId) const;

    //get a refereance to tileTemplate of a specified tileId,
    //this can be freely changed.
    SharedTileTemplate getTileTemplate(int tileId);

signals:
    void tileTemplateAdded(int tileId);
    void tileTemplateRemoved(int tileId);

public slots:

private:
    QList<SharedTileTemplate> mTileTemplates;
};

#endif // TILESET_H
