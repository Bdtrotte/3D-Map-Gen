#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QWidget>


class MapView : public QGraphicsView
{
public:
    MapView(QWidget *parent);
    void clear();
    void createMap(int tx, int ty);

protected slots:
    //scroll to be on copy photoshop

    //pan to be on middleMouse click and drag

    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    float scale;
    int oldX;
    int oldY;
    int MapWidth;
    int MapHeight;
};

#endif // MAPVIEW_H
