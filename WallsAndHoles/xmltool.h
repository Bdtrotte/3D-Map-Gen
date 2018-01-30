#ifndef XMLTOOL_H
#define XMLTOOL_H
#include "tilemap.h"
#include "tiletemplateset.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

class XMLTool
{
public:
    XMLTool(QString path);
    /*
    void openTileMap(TileMap&);
    void openTileTemplateSet();
    */
    int saveTileMap(TileMap& tileMap, TileTemplateSet& templateSet);
    int saveTileTemplateSet(TileTemplateSet& templateSet);

private:
    QString mPath;
};

#endif // XMLTOOL_H
