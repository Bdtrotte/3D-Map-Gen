#ifndef RECTCELL_H
#define RECTCELL_H

#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QBrush>
#include <QPen>


class RectCell : public QGraphicsObject
{
    Q_OBJECT

public:

    //IMPLICIT QOBJECT FUNCITONS//
    explicit RectCell(qreal x, qreal y,qreal w, qreal h, qreal i, qreal j, QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    //END QOBJECT FUNCTIONS//

    void recolor(QColor color);

    void sendTool(qreal i, qreal j, QString string);



signals:
    //slots which send to the parent then are sent to the appropriate RectCells//
    // RectCell(i,j) -- "ayo i'm being hovered on cuh" --> MapView -- "i got you" --> RectCells "oh cool"
    //ALSO qstring is a sit in for our big tool boy class, temp until i add tool bar widget to start
    //scrubbing the bup.
    void emitTool(int i,int j);


public slots:
    //MOUSE EVENTS//
    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e) override;
    //ALL THESE BOYS DO IS CALL RECOLOR//
    void receiveTool();
    //implement later
    //void toolSignal(QColor color, int dist);

private:
        QString mToolName;
        std::tuple<int,int> mCoords;
        std::tuple<int,int> mDimensions;
        std::tuple<int,int> mIndices;
        QBrush mBrush;
        QPen mPen;
};

#endif // RECTCELL_H
