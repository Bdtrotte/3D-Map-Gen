#ifndef XMLTOOL_H
#define XMLTOOL_H

#include "tilemap.h"
#include "tiletemplateset.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

class TileTemplateSetsView;

namespace XMLTool {

    TileMap *openTileMap(QString tileMapPath, TileTemplateSetsView *tileTemplateSetView);
    SharedTileTemplateSet openTileTemplateSet(QString templateSetPath);

    int saveTileMap(TileMap *tileMap, const QList<SharedTileTemplateSet> &tileTemplateSets);
    int saveTileTemplateSet(TileTemplateSet *templateSet);
}

#endif // XMLTOOL_H
