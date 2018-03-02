#include "tilemappreviewgraphicsitem.h"

#include <QPainter>

TileMapPreviewGraphicsItem::TileMapPreviewGraphicsItem()
    : QGraphicsItem(nullptr)
{

}

void TileMapPreviewGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(mColor);
    painter->drawRects(mDrawRegion.rects());
}

QRectF TileMapPreviewGraphicsItem::boundingRect() const
{
    return mDrawRegion.boundingRect();
}

void TileMapPreviewGraphicsItem::setRegion(QRegion region)
{
    prepareGeometryChange();

    region &= mClipRect;
    mDrawRegion = region;
}

void TileMapPreviewGraphicsItem::setColor(const QColor &color)
{
    mColor = color;
    mColor.setAlpha(PREVIEW_ALPHA);
}
