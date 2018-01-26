#ifndef RECTCELL_H
#define RECTCELL_H

#include <QGraphicsItem>
#include <QHoverEvent>

class RectCell : public QGraphicsRectItem
{
public:
    RectCell(qreal x, qreal y, qreal w, qreal h);
    void applyTool(QString temptoolname);

public slots:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);

 private:
    bool mEdited;

signals:
    void toolSignal();
};

#endif // RECTCELL_H
