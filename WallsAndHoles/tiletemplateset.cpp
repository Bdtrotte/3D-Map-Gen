#include "tiletemplateset.h"

TileTemplateSet::TileTemplateSet(QObject *parent)
    : QObject(parent) {}

void TileTemplateSet::addTileTemplate(SharedTileTemplate tileTemplate)
{
    mTileTemplates.append(tileTemplate);

    emit tileTemplateAdded(mTileTemplates.size() - 1);
}

void TileTemplateSet::removeTileTemplate(int index)
{
    Q_ASSERT(index >= 0 && index < mTileTemplates.size());

    mTileTemplates.removeAt(index);

    emit tileTemplateRemoved(index);
}
