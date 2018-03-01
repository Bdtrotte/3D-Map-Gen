#ifndef TILEMAPPREVIEWGRPAHICSITEM_H
#define TILEMAPPREVIEWGRPAHICSITEM_H

#define PREVIEW_ALPHA 100

#include <QGraphicsItem>
#include <QTransform>

class TileMapPreviewGraphicsItem : public QGraphicsItem
{
public:
    TileMapPreviewGraphicsItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;

    void setRegion(QRegion region);
    void setColor(const QColor &color);

    /**
     * @brief setClipRect
     *
     * Any region outside this rect will be clipped.
     * For passing in a tileMap, should not be scaled up.
     * (10x9 tile map should pass in a 10x9 rect)
     *
     * @param rect
     */
    void setClipRect(const QRect &rect) { mClipRect = rect; }

private:
    QRegion mDrawRegion;
    QColor mColor;

    QRect mClipRect;
};

#endif // TILEMAPPREVIEWGRPAHICSITEM_H
