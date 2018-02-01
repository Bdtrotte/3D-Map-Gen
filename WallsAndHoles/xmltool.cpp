#include "xmltool.h"
#include <QDebug>
#include <QMessageBox>

SharedTileMap XMLTool::openTileMap(QString tileMapPath){
    //load the file
    QFile file(tileMapPath);
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Fail to open file:"+tileMapPath;
        return nullptr;
    }
    QXmlStreamReader xmlReader(file.readAll());
    SharedTileMap tileMap;
    //Parse the XML until we reach end of it
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        //If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument) {
                continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader.name() == "TileMap") {
                QSize size;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == QString("width")) {
                         size.setWidth(attr.value().toInt());
                    }
                    if (attr.name().toString() == QString("height")) {
                         size.setHeight(attr.value().toInt());
                    }
                }
                tileMap = SharedTileMap::create(size);
            }
            if(xmlReader.name() == "Tile") {
                float x;
                float y;
                QVector2D position;
                QColor color;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {

                }
                //tileMap->setTile();
            }
        }
    }

    if(xmlReader.hasError()) {
            QMessageBox::critical(0,
            "xmlFile.xml Parse Error",xmlReader.errorString(),
            QMessageBox::Ok);
            return nullptr;
    }
    //close reader and flush file
    xmlReader.clear();
    file.close();
    return tileMap;
}

SharedTileTemplateSet XMLTool::openTileTemplateSet(QString templateSetPath){
    //load the file
    QFile file(templateSetPath);
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Fail to open file:"+templateSetPath;
        return nullptr;
    }
    QXmlStreamReader xmlReader(file.readAll());
    SharedTileTemplateSet templateSet;
    //Parse the XML until we reach end of it
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        //If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument) {
                continue;
        }
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader.name() == "TileTemplateSet") {
                templateSet = SharedTileTemplateSet::create();
            }
            if(xmlReader.name() == "TileTemplate") {
                float thickness;
                float height;
                QVector2D position;
                QColor color;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    qDebug() << attr.name().toString();
                    if (attr.name().toString() == QString("Thickness")) {
                         thickness = attr.value().toFloat();
                    }
                    if (attr.name().toString() == QString("Height")) {
                         height = attr.value().toFloat();
                    }
                    if (attr.name().toString() == QString("Position")) {
                         position = QVector2D(attr.value().toString().split(',')[0].toFloat(),
                                 attr.value().toString().split(',')[1].toFloat());
                    }
                    if (attr.name().toString() == QString("Color")) {
                         color = QColor(attr.value().toString());
                    }
                }
                SharedTileTemplate tileTemplate = SharedTileTemplate::create(height,thickness,position,color);
                templateSet->addTileTemplate(tileTemplate);
            }
        }
    }

    if(xmlReader.hasError()) {
            QMessageBox::critical(0,
            "xmlFile.xml Parse Error",xmlReader.errorString(),
            QMessageBox::Ok);
            return nullptr;
    }
    //close reader and flush file
    xmlReader.clear();
    file.close();
    return templateSet;
}

int XMLTool::saveTileMap(TileMap &tileMap, TileTemplateSet &templateSet, QString tileMapPath, QString templateSetPath){
    const QVector<SharedTileTemplate> templates = templateSet.cTileTemplates();
    const Array2D<QSharedPointer<Tile>> tiles = tileMap.cTiles();
    QFile file(tileMapPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return -1;
    QTextStream out(&file);
    QDomDocument doc;
    QDomElement element;
    QDomAttr attr;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    doc.appendChild(instruction);

    QDomElement root = doc.createElement( "TileMap" );
    attr = doc.createAttribute("width");
    attr.setValue(QString::number(tiles.size().width()));
    root.setAttributeNode(attr);
    attr = doc.createAttribute("height");
    attr.setValue(QString::number(tiles.size().height()));
    root.setAttributeNode(attr);
    attr = doc.createAttribute("templateSet");
    attr.setValue(templateSetPath);
    root.setAttributeNode(attr);
    doc.appendChild(root);
    for(int i=0; i<tiles.size().height(); i++){
        for(int j=0; j<tiles.size().width(); j++){
            QSharedPointer<Tile> tile = tiles(i,j);
            if(!tile->isEmpty()){
                element = doc.createElement("Tile");
                attr = doc.createAttribute("X");
                attr.setValue(QString::number(j));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("Y");
                attr.setValue(QString::number(i));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativeThickness");
                attr.setValue(QString::number(tile->relativeThickness()));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativeHeight");
                attr.setValue(QString::number(tile->relativeHeight()));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativePosition");
                attr.setValue(QString("%1,%2").arg(
                    QString::number(tile->relativePosition()[0]),
                    QString::number(tile->relativePosition()[1])));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("TemplateID");
                attr.setValue(QString::number(templates.indexOf(tile->tileTemplate())));
                element.setAttributeNode(attr);
                root.appendChild(element);
            }
        }
    }
    doc.save(out, 4);
    XMLTool::saveTileTemplateSet(templateSet, templateSetPath);
    return 0;
}

int XMLTool::saveTileTemplateSet(TileTemplateSet &templateSet, QString templateSetPath){
    const QVector<SharedTileTemplate> templatelist = templateSet.cTileTemplates();
    QFile file(templateSetPath);
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
