#include "rendermap.h"
#include <QDebug>

RenderMap::RenderMap(QObject *parent)
    :QObject(parent){ }

void RenderMap::renderMap(int a, MapCell *mapCell){
    switch (a) {
    case heightMap:
    {
        float h = mapCell->getTileHeight();
        if(h < 0){
            float sig = (.25*h)/(.25*(h - 1));
            int colorVal = 255-(255*sig);
            mapCell->setView(QColor(255, colorVal, colorVal, 255));
        }
        else{
            //if height is greater than 0 the heightMap will be green
            float sig = (.25*h)/(.25*(h + 1));
            int colorVal = 255-(255*sig);
            mapCell->setView(QColor(colorVal, 255, colorVal, 255));
        }
        break;
    }
    case defaultView:
    {
        mapCell->setView(Qt::transparent);
    }
    default:
        break;
    }
}
