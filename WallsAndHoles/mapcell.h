#ifndef MAPCELL_H
#define MAPCELL_H

#include "mapcellgraphicsitem.h"
#include "tile.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsRectItem>

class MapCell : public QObject
{
    Q_OBJECT

public:

    MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent = nullptr);
    ~MapCell();

    void setGraphics(MapViewMode viewMode, bool enabled);

public slots:
    void tileChanged();

private:
    QGraphicsScene *mScene;

    // TODO replace this with a gridGraphicsItem in MapView
    QGraphicsRectItem *mGrid;
    QGraphicsRectItem *mBackground;
    MapCellGraphicsItem *mGraphics;
};

#endif // MAPCELL_H
