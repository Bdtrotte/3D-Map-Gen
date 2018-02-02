#include "rendermap.h"
#include <QDebug>

RenderMap::RenderMap(QObject *parent)
    :QObject(parent){ }

void RenderMap::test(int a, Array2D<MapCell *> mapCells){
    switch (a) {
    case heightMap:
        if(mapCells.size() == QSize(0,0)){
            qDebug() << "Not";
            break;
        }
        else
            qDebug() << "HeightMap";
        break;
    case defaultView:
        if(mapCells.size() == QSize(0,0)){
            qDebug() << "Not";
            break;
        }
        else
            qDebug() << "Default";
        break;
    default:
        break;
    }
}
