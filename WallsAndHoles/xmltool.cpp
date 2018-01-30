#include "xmltool.h"
#include <QDebug>

XMLTool::XMLTool(QString path)
{
    mPath = path;
}

int XMLTool::saveTileMap(TileMap &tileMap, TileTemplateSet &templateSet){

}

int XMLTool::saveTileTemplateSet(TileTemplateSet &templateSet){
    const QVector<SharedTileTemplate> templatelist = templateSet.cTileTemplates();
    QFile file(mPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return -1;
    QTextStream out(&file);
    QDomDocument doc;
    QDomElement element;
    QDomAttr attr;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    doc.appendChild(instruction);

    QDomElement root = doc.createElement( "TileTemplateSet" );
    doc.appendChild(root);
    for(SharedTileTemplate temp: templatelist){
        element = doc.createElement("TileTemplate");
        attr = doc.createAttribute("Height");
        attr.setValue(QString::number(temp->height()));
        element.setAttributeNode(attr);
        attr = doc.createAttribute("Thickness");
        attr.setValue(QString::number(temp->thickness()));
        element.setAttributeNode(attr);
        attr = doc.createAttribute("Position");
        attr.setValue(QString("%1,%2").arg(QString::number(temp->position()[0]), QString::number(temp->position()[1])));
        element.setAttributeNode(attr);
        attr = doc.createAttribute("Color");
        attr.setValue(temp->color().name());
        element.setAttributeNode(attr);
        root.appendChild(element);
    }
    doc.save(out, 4);
    return 0;
}
