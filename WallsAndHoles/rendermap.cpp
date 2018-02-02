#include "rendermap.h"
#include <QDebug>

RenderMap::RenderMap(QObject *parent)
    :QObject(parent){ }

void RenderMap::test(int a){
    switch (a) {
    case heightMap:
        qDebug() << "HeightMap";
        break;
    case defaultView:
        qDebug() << "DefaultView";
    default:
        break;
    }
}
