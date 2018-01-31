#ifndef MAPCELL_H
#define MAPCELL_H

#include "tile.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

class MapCell : public QObject
{
    Q_OBJECT

public:
    MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent = nullptr);
    ~MapCell();

    void setHighlightBrush(const QBrush &color) { mHighlight->setBrush(color); }

    void changeHeightMap(int x, int y, QColor color);
public slots:
    void tileChanged();

private:
    QGraphicsScene *mScene;
    int mX, mY;

    //the tile this cell is displaying.
    const Tile &mTile;

    QGraphicsRectItem *mGraphics;
    QGraphicsRectItem *mHighlight;
    QGraphicsRectItem *mBackground;
    QGraphicsRectItem *mHeightMap;
};

#endif // MAPCELL_H
