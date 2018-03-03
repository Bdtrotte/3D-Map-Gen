#ifndef TILEPROPERTYMANAGER_H
#define TILEPROPERTYMANAGER_H

#include "abstractpropertymanager.h"

#include "tile.h"

#include <QVector>
#include <QDebug>

/**
 * @brief The TilePropertyManager class
 * Property manager for a Tile.
 */
class TilePropertyManager : public AbstractPropertyManager
{
    Q_OBJECT

public:
    TilePropertyManager(QVector<Tile *> tiles);

    void propertyEdited(QString propertyName, QVariant value) override;

    QVector<QVector<QVariant>> properties() override;

private:
    QVector<Tile *> mTiles;
};

#endif // TILEPROPERTYMANAGER_H
