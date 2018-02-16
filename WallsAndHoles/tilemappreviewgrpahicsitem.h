#ifndef TILEMAPPREVIEWGRPAHICSITEM_H
#define TILEMAPPREVIEWGRPAHICSITEM_H

#define PREVIEW_ALPHA 100

#include <QGraphicsItem>
#include <QTransform>

class TileMapPreviewGrpahicsItem : public QGraphicsItem
{
public:
    TileMapPreviewGrpahicsItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;

    void setRegion(const QRegion &region);
    void setColor(const QColor &color);

private:
    QRegion mDrawRegion;
    QColor mColor;
};

#endif // TILEMAPPREVIEWGRPAHICSITEM_H
