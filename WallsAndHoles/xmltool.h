#ifndef XMLTOOL_H
#define XMLTOOL_H
#include "tilemap.h"
#include "tiletemplateset.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

namespace XMLTool {

    SharedTileMap openTileMap(QString tileMapPath);
    SharedTileTemplateSet openTileTemplateSet(QString templateSetPath);

    int saveTileMap(SharedTileMap tileMap, bool saveTemplates = true);

    int saveTileTemplateSet(TileTemplateSet *templateSet);
}

#endif // XMLTOOL_H
