#ifndef TILEMAPHELPERS_H
#define TILEMAPHELPERS_H

#include "tilemap.h"

#include <QStack>
#include <QSet>

// QPoints are not hashable in Qt by default!
inline uint qHash (const QPoint & key)
{
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

namespace TileMapHelper {

/**
 * @brief getFillRegion
 *
 * The region of tiles that are of the same tiletemplate of the tile at x y
 * and touch this region, or another tile in this region.
 *
 * The output is what one would expect from a fill tool.
 *
 * @param tilemap
 * @param x
 * @param y
 * @return
 */
QRegion getFillRegion(TileMap *tileMap, int x, int y);

QRegion getAllOfTemplate(TileMap *tileMap, TileTemplate *tileTemplate);
QRegion getAllOfTemplateAtTile(TileMap *tileMap, int x, int y);

}

#endif // TILEMAPHELPERS_H
