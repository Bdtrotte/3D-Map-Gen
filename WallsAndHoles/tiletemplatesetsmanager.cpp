#include "tiletemplatesetsmanager.h"

TileTemplateSetsManager::TileTemplateSetsManager(TileMap *tileMap,
                                                 QObject *parent)
    : QObject(parent)
    , mTileMap(tileMap) {}

void TileTemplateSetsManager::newTileTemplateSet()
{

}

void TileTemplateSetsManager::addTileTemplateSet(SavableTileTemplateSet *tileTemplateSet)
{
    tileTemplateSet->setParent(this);

    mTileTemplateSets.append(tileTemplateSet);

    emit tileTemplateSetAdded(tileTemplateSet);
}

bool TileTemplateSetsManager::removeTileTemplateSet(SavableTileTemplateSet *TileTemplateSet)
{
    int

    emit tileTemplateSetAboutToBeRemoved(tileTe);
}

bool TileTemplateSetsManager::removeTileTemplateSet(int index)
{

}

void TileTemplateSetsManager::saveAllTileTemplateSets()
{

}

SavableTileTemplateSet *TileTemplateSetsManager::loadTileTemplateSet(QString path)
{

}
