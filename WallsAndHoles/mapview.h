#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QWidget>


class MapView : public QGraphicsView
{
public:
    MapView(QWidget *parent);
protected slots:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    float scale;
    int oldX;
    int oldY;
};

#endif // MAPVIEW_H
