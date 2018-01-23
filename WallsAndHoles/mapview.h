#ifndef MAPVIEW_H
#define MAPVIEW_H

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

    void setupGrid();

    void clear();

    void addTile();

    void paintEvent(QPaintEvent *painter);

private:
    Ui::MapView *ui;
};

#endif // MAPVIEW_H
