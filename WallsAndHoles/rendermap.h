#ifndef RENDERMAP_H
#define RENDERMAP_H
#include <QObject>
#include "mapcell.h"

class RenderMap : public QObject{
        Q_OBJECT

public:
    RenderMap(QObject *parent = nullptr);
    enum Views{ defaultView, heightMap };
    void renderMap(int view, MapCell *mapCell);
};

#endif // RENDERMAP_H
