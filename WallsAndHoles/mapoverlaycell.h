#ifndef MAPOVERLAYCELL_H
#define MAPOVERLAYCELL_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>

class MapOverlayCell {
public:
    MapOverlayCell(QGraphicsScene *scene, int x, int y, QColor color);
    ~MapOverlayCell();

protected:
    QGraphicsScene *mScene;
    QGraphicsRectItem *mRect;
};

#endif // MAPOVERLAYCELL_H
