#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
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
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    float scale;
    int MapWidth;
    int MapHeight;
    int KeyDown;
};

#endif // MAPVIEW_H
