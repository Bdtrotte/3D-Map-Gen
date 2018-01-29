#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QWidget>
#include "tilemap.h"
#include "tiletemplateset.h"
#include "xmltool.h"


class MapView : public QGraphicsView
{
public:
    MapView(QWidget *parent);
    void clear();
    void createMap(int tx, int ty);
    void openMap(QString path);
    void saveMap(QString path);

protected slots:
    //scroll to be on copy photoshop

    //pan to be on middleMouse click and drag

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
private:
    float mScale;
    int mMapWidth;
    int mMapHeight;
    int mOldX;
    int mOldY;
};

#endif // MAPVIEW_H
