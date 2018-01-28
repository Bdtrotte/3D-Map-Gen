#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QWidget>
#include <QObject>
#include "rectcell.h"


class MapView : public QGraphicsView
{

    Q_OBJECT

public:
    MapView(QWidget *parent);
    void clear();
    void createMap(int tx, int ty);

signals:
   void emitRect(int i , int j);

protected slots:
    void receiveRectCell(int i, int j);
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    float mScale;
    int mMapWidth;
    int mMapHeight;
    int mOldX;
    int mOldY;

};

#endif // MAPVIEW_H
