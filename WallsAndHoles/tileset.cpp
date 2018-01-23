#include "tileset.h"

TileSet::TileSet(QObject *parent)
    : QObject(parent) {}

void TileSet::addTileTemplate(TileTemplate *tileTemplate)
{
    tileTemplate->setTileId(mTileTemplates.size());
    mTileTemplates.append(SharedTileTemplate(tileTemplate));
    connect(tileTemplate, &TileTemplate::propertyChanged,
            this, &TileSet::tileTemplateChanged);

    emit tileTemplateAdded(mTileTemplates.size() - 1);
}

void TileSet::removeTileTemplate(int index)
{
    Q_ASSERT(index >= 0 && index < mTileTemplates.size());

    mTileTemplates.removeAt(index);

    //updates the tileId of all templates
    for (auto it = mTileTemplates.begin() + index; it != mTileTemplates.end(); ++it) {
        (*it)->setTileId(index++);
    }

    emit tileTemplateRemoved(index);
}

const SharedTileTemplate TileSet::getCTileTemplate(int tileId) const
{
    Q_ASSERT(tileId >= 0 && tileId < mTileTemplates.size());

    return mTileTemplates[tileId];
}

SharedTileTemplate TileSet::getTileTemplate(int tileId)
{
    Q_ASSERT(tileId >= 0 && tileId < mTileTemplates.size());

    return mTileTemplates[tileId];
}
