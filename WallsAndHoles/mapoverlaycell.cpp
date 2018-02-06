#include "mapoverlaycell.h"


MapOverlayCell::MapOverlayCell(QGraphicsScene *scene, int x, int y, QColor color)
    : mScene(scene)
{
    Q_ASSERT( mScene != nullptr );

    mRect = new QGraphicsRectItem(x*30, y*30, 30, 30);
    mRect->setBrush(QBrush(color));
    mRect->setPen(Qt::NoPen);

    mScene->addItem(mRect);
}


MapOverlayCell::~MapOverlayCell() {
    mScene->removeItem(mRect);
    delete mRect;
}
