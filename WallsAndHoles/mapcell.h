#ifndef MAPCELL_H
#define MAPCELL_H

#include "mapcellgraphicsitem.h"
#include "tile.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>

class MapCell : public QObject
{
    Q_OBJECT

public:

    MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent = nullptr);
    ~MapCell();

    void setHighlightBrush(const QBrush &color) { mHighlight->setBrush(color); }

    float getTileHeight();

    void setGraphics(int flag);

public slots:
    void tileChanged();

private:

    QGraphicsScene *mScene;
    int mX, mY;

    //the tile this cell is displaying.
    const Tile &mTile;

    enum views { defaultView, heightMapView };
    int mViewFlag;

    MapCellGraphicsItem *mBackground;
    MapCellGraphicsItem *mHighlight;
    MapCellGraphicsItem *mGraphics;

};

#endif // MAPCELL_H
