#include "tilemaphelpers.h"

QRegion TileMapHelper::getFillRegion(TileMap *tileMap, int x, int y)
{
    if (!tileMap || !tileMap->contains(x, y)) return QRegion();

    TileTemplate *tileTemplate = tileMap->tileAt(x, y).tileTemplate();

    QStack<QPoint> toExplore;
    QSet<QPoint> inStackorExplored;
    QRegion region;

    toExplore.push(QPoint(x, y));

    while (!toExplore.isEmpty()) {
        QPoint p = toExplore.pop();
        inStackorExplored.insert(p);
        region += QRect(p, QSize(1, 1));

        QPoint neighbors[4] = {
            p + QPoint(1, 0),
            p + QPoint(-1, 0),
            p + QPoint(0, 1),
            p + QPoint(0, -1)
        };

        for (int i = 0; i < 4; ++i) {
            QPoint next = neighbors[i];
            if (tileMap->contains(next.x(), next.y())
                    && tileMap->tileAt(next.x(), next.y()).tileTemplate() == tileTemplate
                    && !inStackorExplored.contains(next)) {
                toExplore.push(next);
                inStackorExplored.insert(next);
            }
        }
    }

    return region;
}

QRegion TileMapHelper::getAllOfTemplate(TileMap *tileMap, TileTemplate *tileTemplate)
{
    if (!tileMap) return QRegion();

    QRegion region;

    for (int w = 0; w < tileMap->width(); ++w)
        for (int h = 0; h < tileMap->height(); ++h)
            if (tileMap->tileAt(w, h).tileTemplate() == tileTemplate)
                region += QRect(w, h, 1, 1);

    return region;
}

QRegion TileMapHelper::getAllOfTemplateAtTile(TileMap *tileMap, int x, int y)
{
    if (!tileMap || !tileMap->contains(x, y)) return QRegion();

    TileTemplate *tileTemplate = tileMap->tileAt(x, y).tileTemplate();

    return getAllOfTemplate(tileMap, tileTemplate);
}
