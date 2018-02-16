#include "tilemappreviewgrpahicsitem.h"

#include <QPainter>

TileMapPreviewGrpahicsItem::TileMapPreviewGrpahicsItem()
    : QGraphicsItem(nullptr) {}

void TileMapPreviewGrpahicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    painter->setBrush(mColor);

    painter->drawRects(mDrawRegion.rects());

    painter->restore();
}
