#ifndef GRID_H
#define GRID_H
#include <QGraphicsView>

class Grid : public QGraphicsView
{
public:
    Grid(QWidget *parent);

protected:
   // void mouseMoveEvent(QMouseEvent *e);
   // void mouseReleaseEvent(QMouseEvent *e);
   // void mousePressEvent(QMouseEvent *e);

private:
    bool mouseDown;
    int prevX;
    int prevY;
};

#endif // GRID_H
