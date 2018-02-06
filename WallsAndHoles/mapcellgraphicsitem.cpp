#include "mapcellgraphicsitem.h"

MapCellGraphicsItem::MapCellGraphicsItem(int x, int y, qreal w, qreal h, const Tile &tile)
    :
      mX(x),
      mY(y),
      mW(w),
      mH(h),
      mTile(tile),
      mViewFlag(1)
{
    mRect = boundingRect();
}
void MapCellGraphicsItem::setBrush(QBrush brush){
    mBrush = brush;
}

void MapCellGraphicsItem::setPen(QPen pen){
    mPen = pen;
}

void MapCellGraphicsItem::setView(int viewFlag){
    mViewFlag = viewFlag;
}

QRectF MapCellGraphicsItem::boundingRect() const
{
    return QRectF(mX, mY, mW, mH);
}

void MapCellGraphicsItem::setRect(int x, int y, qreal w, qreal h){
    mX = x;
    mY = y;
    mW = w;
    mH = h;
}



void MapCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){   
    switch (mViewFlag) {
    case Noview:
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::NoPen);
        painter->drawRect(mRect);
        update();
        break;
    }
    case DefaultView:
    {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
        painter->drawRect(mRect);
        update();
        break;
    }
    case HeightMap:
    {
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
        painter->drawRect(mX-10, mY-10, 30, 30);
        update();
    }
    default:
        break;
    }
}

