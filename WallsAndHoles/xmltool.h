#ifndef XMLTOOL_H
#define XMLTOOL_H

#include "tilemap.h"
#include "savabletiletemplateset.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>

class TileTemplateSetsManager;

namespace XMLTool {

enum SaveErrors {
    NoError,
    OpenFileError
};

TileMap *openTileMap(QString tileMapPath, TileTemplateSetsManager *tileTemplateSetManager);
SavableTileTemplateSet *openTileTemplateSet(QString templateSetPath);

int saveTileMap(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets);
int saveTileTemplateSet(SavableTileTemplateSet *templateSet);

}

#endif // XMLTOOL_H
