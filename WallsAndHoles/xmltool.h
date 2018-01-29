#ifndef XMLTOOL_H
#define XMLTOOL_H
#include "tilemap.h"
#include "tiletemplateset.h"

class XMLTool
{
public:
    XMLTool(QString path);
    /*
    void openTileMap(TileMap&);
    void openTileTemplateSet();
    */
    void saveTileMap(TileMap& tileMap, TileTemplateSet& templateSet);
    void saveTileTemplateSet(TileTemplateSet& templateSet);

private:
    QString mPath;
};

#endif // XMLTOOL_H
