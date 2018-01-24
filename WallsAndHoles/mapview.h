#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QResizeEvent>
#include <QWidget>

namespace Ui {
class MapView;
}

class MapView : public QWidget
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent = 0);

    ~MapView();

private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MapView *ui;
    bool mouseDown;
    int initX;
    int initY;

};

#endif // MAPVIEW_H
