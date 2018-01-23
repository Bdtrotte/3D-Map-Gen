#ifndef RECTCELL_H
#define RECTCELL_H

#include <QGraphicsItem>
#include <QHoverEvent>

class rectCell : public QGraphicsRectItem
{
public:
    rectCell(qreal x, qreal y, qreal w, qreal h);
    //link

    //mouseover

    //onclick

    //rightclick


public slots:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
};

#endif // RECTCELL_H
