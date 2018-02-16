#ifndef TILEMAPPREVIEWGRPAHICSITEM_H
#define TILEMAPPREVIEWGRPAHICSITEM_H

#define PREVIEW_ALPHA 40

#include <QGraphicsItem>

class TileMapPreviewGrpahicsItem : public QGraphicsItem
{
public:
    TileMapPreviewGrpahicsItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override { return mDrawRegion.boundingRect(); }

    void setRegion(const QRegion &region) { mDrawRegion = region; update(); }
    void setColor(const QColor &color) { mColor = color; mColor.setAlpha(PREVIEW_ALPHA); update(); }

private:
    QRegion mDrawRegion;
    QColor mColor;
};

#endif // TILEMAPPREVIEWGRPAHICSITEM_H
