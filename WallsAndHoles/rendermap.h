#ifndef RENDERMAP_H
#define RENDERMAP_H
#include <QObject>
#include "tile.h"
#include "tilemap.h"
#include "mapcell.h"
#include "array2d.h"

class RenderMap : public QObject{
        Q_OBJECT

public:
    RenderMap(QObject *parent = nullptr);
    enum Views{ defaultView, heightMap };
    void renderMap(int a, MapCell *mapCell);
};

#endif // RENDERMAP_H
