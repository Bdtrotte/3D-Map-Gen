#include "rendermap.h"
#include <QDebug>

RenderMap::RenderMap(QObject *parent)
    :QObject(parent){ }

void RenderMap::renderMap(int view, MapCell *mapCell){
    switch (view){
    case heightMap:
    {
        float height = mapCell->getTileHeight();
        if(height < 0){
            //if the height is less than 0 the heightMap will be red
            float sig = (.25*height)/(.25*(height - 1));
            int colorVal = 255-(255*sig);
            int alpha = 150*sig;
            mapCell->setView(QColor(255, colorVal, colorVal, alpha));
        }
        else{
            //if height is greater than 0 the heightMap will be green
            float sig = (.25*height)/(.25*(height + 1));
            int colorVal = 255-(255*sig);
            int alpha = 150*sig;
            mapCell->setView(QColor(colorVal, 255, colorVal, alpha));
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
