#ifndef RECTCELL_H
#define RECTCELL_H

#include <QGraphicsItem>
#include <QHoverEvent>

class rectCell : public QGraphicsRectItem
{
public:
    rectCell(qreal x, qreal y, qreal w, qreal h);

public slots:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
 private:
    bool edited;
};

#endif // RECTCELL_H
