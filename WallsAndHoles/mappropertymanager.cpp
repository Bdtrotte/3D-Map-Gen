#include "mappropertymanager.h"

MapPropertyManager::MapPropertyManager(TileMap *tileMap)
    : AbstractPropertyManager()
    , mTileMap(tileMap) {}

void MapPropertyManager::propertyEdited(QString name, QVariant value)
{
    if (name == "Width") {
        QSize size = mTileMap->mapSize();
        size.setWidth(value.toInt());
        mTileMap->resizeMap(size);
    } else if (name == "Height") {
        QSize size = mTileMap->mapSize();
        size.setHeight(value.toInt());
        mTileMap->resizeMap(size);
    }
}

QVector<QVector<QVariant> > MapPropertyManager::properties()
{
    // TODO: Make all these editable
    return {
        {"Width",       mTileMap->mapSize().width(),  true, 1, 1000},
        {"Height",      mTileMap->mapSize().height(), true, 1, 1000},
        {"Is Indoors",  mTileMap->isIndoor(),         false},
        {"Has Ceiling", mTileMap->hasCeiling(),       false}
    };
}
