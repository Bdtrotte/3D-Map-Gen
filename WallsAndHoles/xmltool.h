#ifndef XMLTOOL_H
#define XMLTOOL_H

#include "tilemap.h"
#include "savabletiletemplateset.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

class TileTemplateSetsView;

namespace XMLTool {

    TileMap *openTileMap(QString tileMapPath, TileTemplateSetsView *tileTemplateSetView);
    SavableTileTemplateSet *openTileTemplateSet(QString templateSetPath);

    int saveTileMap(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets);
    int saveTileTemplateSet(SavableTileTemplateSet *templateSet);
}

#endif // XMLTOOL_H
