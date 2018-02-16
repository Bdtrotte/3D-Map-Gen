#include "tilemappreviewgrpahicsitem.h"

#include <QPainter>

TileMapPreviewGrpahicsItem::TileMapPreviewGrpahicsItem()
    : QGraphicsItem(nullptr)
{

}

void TileMapPreviewGrpahicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(mColor);
    painter->drawRects(mDrawRegion.rects());
}

QRectF TileMapPreviewGrpahicsItem::boundingRect() const
{
    return mDrawRegion.boundingRect();
}

void TileMapPreviewGrpahicsItem::setRegion(const QRegion &region)
{
    prepareGeometryChange();

    QTransform t;
    t.scale(30, 30);
    mDrawRegion = t.map(region);
}

void TileMapPreviewGrpahicsItem::setColor(const QColor &color)
{
    mColor = color;
    mColor.setAlpha(PREVIEW_ALPHA);
}
