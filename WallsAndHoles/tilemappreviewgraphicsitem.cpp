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

void TileMapPreviewGraphicsItem::setRegion(const QRegion &region)
{
    prepareGeometryChange();

    QTransform t;
    t.scale(30, 30);
    mDrawRegion = t.map(region);
}

void TileMapPreviewGraphicsItem::setColor(const QColor &color)
{
    mColor = color;
    mColor.setAlpha(PREVIEW_ALPHA);
}
