#ifndef MAPCELL_H
#define MAPCELL_H

#include "tile.h"

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>

class MapCell : public QObject
{
    Q_OBJECT

public:
    MapCell(QGraphicsScene *scene, int x, int y, const Tile &tile, QObject *parent = nullptr);
    ~MapCell();

    void setHighlightBrush(const QBrush &color) { mHighlight->setBrush(color); }

    float getTileHeight();
    void setView(QColor color);

public slots:
    void tileChanged();
signals:
    void cellUpdated();
private:
    QGraphicsScene *mScene;
    int mX, mY;

    //the tile this cell is displaying.
    const Tile &mTile;

    QGraphicsRectItem *mGraphics;
    QGraphicsRectItem *mHighlight;
    QGraphicsRectItem *mBackground;
    QGraphicsRectItem *mView;
};

#endif // MAPCELL_H
