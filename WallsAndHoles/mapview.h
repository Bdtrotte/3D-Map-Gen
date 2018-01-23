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


public slots:
    void resizeEvent(QResizeEvent *q);

private:
    Ui::MapView *ui;
};

#endif // MAPVIEW_H
