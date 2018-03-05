#include "mapcellgraphicsitem.h"

MapCellGraphicsItem::MapCellGraphicsItem(int x, int y, qreal w, qreal h, const Tile &tile)
    : mTile(tile)
    , mViewMode(DefaultView)
    , mRect(QRectF(x, y, w, h)) {}

void MapCellGraphicsItem::setViewMode(int viewMode)
{
    mViewMode = viewMode;
}

QRectF MapCellGraphicsItem::boundingRect() const
{
    return mRect;
}

void MapCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (mViewMode == NoView) return;

    if (mViewMode & DefaultView) {
        if (mTile.hasTileTemplate()) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(mTile.tileTemplate()->color());
            painter->drawRect(mRect);
        }
    }

    if (mViewMode & HeightMapView) {
        float height = mTile.height();
        if(height < 0){
            float sig =(.25*height)/(.25*height-1);
            int colorVal = 255-(255*sig);
            int alpha = 255*sig;
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(QColor(255, colorVal, colorVal, alpha)));
        }
        else{
            float sig =(.25*height)/(.25*height+1);
            int colorVal = 255-(255*sig);
            int alpha = 255*sig;
            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(QColor(colorVal, 255, colorVal, alpha)));
        }
        painter->drawRect(mRect);
    }
}

