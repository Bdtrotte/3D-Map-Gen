#ifndef MAPPROPERTYMANAGER_H
#define MAPPROPERTYMANAGER_H

#include "abstractpropertymanager.h"
#include "tilemap.h"

class MapPropertyManager : public AbstractPropertyManager
{
    Q_OBJECT

public:
    MapPropertyManager(TileMap *tileMap);

    void propertyEdited(QString, QVariant) override;

    QVector<QVector<QVariant>> properties() override;

private:
    TileMap *mTileMap;
};

#endif // MAPPROPERTYMANAGER_H
