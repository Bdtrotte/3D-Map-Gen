#ifndef XMLTOOL_H
#define XMLTOOL_H
#include "tilemap.h"
#include "tiletemplateset.h"

class XMLTool
{
public:
    XMLTool(QString path);
    openTileMap(TileMap&);
    openTileTemplateSet();
    saveTileMap();
    saveTileTemplateSet();
};

#endif // XMLTOOL_H
