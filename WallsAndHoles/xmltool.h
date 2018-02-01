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

    int saveTileMap(TileMap& tileMap, TileTemplateSet& templateSet, QString tileMapPath, QString templateSetPath);
    int saveTileTemplateSet(TileTemplateSet& templateSet, QString templateSetPath);
}

#endif // XMLTOOL_H
