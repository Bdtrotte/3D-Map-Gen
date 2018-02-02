#ifndef RENDERMAP_H
#define RENDERMAP_H
#include <QObject>
#include "tile.h"
#include "tilemap.h"

class RenderMap : public QObject{
        Q_OBJECT

public:
    RenderMap(QObject *parent = nullptr);
    enum Views{ defaultView, heightMap };
    void test(int a);

public slots:
    //void tileChanged();


};

#endif // RENDERMAP_H
