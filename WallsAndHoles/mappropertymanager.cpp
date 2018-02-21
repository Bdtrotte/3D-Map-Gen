#include "mappropertymanager.h"

MapPropertyManager::MapPropertyManager(TileMap *tileMap)
    : AbstractPropertyManager()
    , mTileMap(tileMap) {}

void MapPropertyManager::propertyEdited(QString, QVariant)
{

}

QVector<QVector<QVariant> > MapPropertyManager::properties()
{
    // TODO: Make all these editable
    return {
        {"Width",       mTileMap->mapSize().width(),  false, 1, 1000},
        {"Height",      mTileMap->mapSize().height(), false, 1, 1000},
        {"Is Indoors",  mTileMap->isIndoor(),         false},
        {"Has Ceiling", mTileMap->hasCeiling(),       false}
    };
}
