#include "xmltool.h"

#include "tiletemplatesetsview.h"

#include <QDebug>
#include <QMessageBox>

TileMap *XMLTool::openTileMap(QString tileMapPath, TileTemplateSetsView *tileTemplateSetView)
{
    //load the file
    QFile file(tileMapPath);
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Fail to open file:"+tileMapPath;
        return nullptr;
    }
    QXmlStreamReader xmlReader(file.readAll());

    TileMap *tileMap;

    QVector<SharedTileTemplateSet> loadedTileTemplateSets;

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
                tileMap = new TileMap(size);
            }

            if(xmlReader.name()=="TileTemplateSet"){
                SharedTileTemplateSet templateSet;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == QString("SavePath")) {
                        templateSet = XMLTool::openTileTemplateSet(attr.value().toString());
                    }
                }
                if(templateSet==nullptr)
                    return nullptr;

                tileTemplateSetView->addTileTemplateSet(templateSet);
                loadedTileTemplateSets.append(templateSet);
            }

            if(xmlReader.name() == "Tile") {
                float x;
                float y;
                int id;
                SharedTileTemplate tileTemplate;
                float relativeThickness;
                float relativeHeight;
                QVector2D relativePosition;
                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if(attr.name().toString() == QString("x")){
                        x = attr.value().toInt();
                    }
                    if(attr.name().toString() == QString("y")){
                        y = attr.value().toInt();
                    }
                    if(attr.name().toString() == QString("TemplateID")){
                        id = attr.value().toInt();
                    }
                    if(attr.name().toString() == QString("RelativeThickness")){
                        relativeThickness = attr.value().toFloat();
                    }
                    if(attr.name().toString() == QString("RelativeHeight")){
                        relativeHeight = attr.value().toFloat();
                    }
                    if(attr.name().toString() == QString("RelativePosition")){
                        QStringList pos = attr.value().toString().split(',');
                        relativePosition[0] = pos[0].toFloat();
                        relativePosition[1] = pos[1].toFloat();
                    }
                }

                for (SharedTileTemplateSet set: loadedTileTemplateSets) {
                    if (id >= set->size()) {
                        id -= set->size();
                        continue;
                    } else {
                        tileTemplate = set->cTileTemplates()[id];
                        break;
                    }
                }
                Tile& tile = tileMap->tileAt(x,y);
                tile.resetTile(tileTemplate);
                tile.setRelativeThickness(relativeThickness);
                tile.setRelativeHeight(relativeHeight);
                tile.setRelativePosition(relativePosition);
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
                templateSet = SharedTileTemplateSet::create(templateSetPath,
                                                            xmlReader.attributes()[0].value().toString(),
                                                            true);
            }
            if(xmlReader.name() == "TileTemplate") {
                QString name;
                float thickness;
                float height;
                QVector2D position;
                QColor color;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name() == "Name")
                        name = attr.value().toString();

                    if (attr.name().toString() == QString("Thickness"))
                        thickness = attr.value().toFloat();

                    if (attr.name().toString() == QString("Height"))
                        height = attr.value().toFloat();

                    if (attr.name().toString() == QString("Position")) {
                        QStringList pos = attr.value().toString().split(',');
                        position[0] = pos[0].toFloat();
                        position[1] = pos[1].toFloat();
                    }

                    if (attr.name().toString() == QString("Color"))
                        color = QColor(attr.value().toString());
                }
                SharedTileTemplate tileTemplate = SharedTileTemplate::create(color, name, height,thickness,position);
                templateSet->addTileTemplate(tileTemplate, true);
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

int XMLTool::saveTileMap(TileMap *tileMap, const QList<SharedTileTemplateSet> &tileTemplateSets) {
    QVector<SharedTileTemplateSet> usedTileTemplateSets;
    QString tileMapPath = tileMap->savePath();
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
    attr.setValue(QString::number(tileMap->mapSize().width()));
    root.setAttributeNode(attr);
    attr = doc.createAttribute("height");
    attr.setValue(QString::number(tileMap->mapSize().height()));
    root.setAttributeNode(attr);
    doc.appendChild(root);

    for (SharedTileTemplateSet tts : tileTemplateSets) {
        if (tileMap->tileTemplateSetUsed(tts)) {
            usedTileTemplateSets.append(tts);

            element = doc.createElement("TileTemplateSet");
            element.setAttributeNode(attr);
            attr = doc.createAttribute("SavePath");
            attr.setValue(tts->savePath());
            element.setAttributeNode(attr);
            root.appendChild(element);
        }
    }

    for (int y = 0; y < tileMap->mapSize().height(); ++y) {
        for (int x = 0; x < tileMap->mapSize().width(); ++x) {
            const Tile &tile = tileMap->cTileAt(x, y);

            if(tile.hasTileTemplate()) {
                element = doc.createElement("Tile");
                attr = doc.createAttribute("x");
                attr.setValue(QString::number(x));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("y");
                attr.setValue(QString::number(y));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativeThickness");
                attr.setValue(QString::number(tile.relativeThickness()));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativeHeight");
                attr.setValue(QString::number(tile.relativeHeight()));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("RelativePosition");
                attr.setValue(QString("%1,%2").arg(
                    QString::number(tile.relativePosition()[0]),
                    QString::number(tile.relativePosition()[1])));
                element.setAttributeNode(attr);
                attr = doc.createAttribute("TemplateID");
                int id=-1; int count=0;
                for (SharedTileTemplateSet set: usedTileTemplateSets) {
                    if ((id=set->cTileTemplates().indexOf(tile.tileTemplate())) == -1) {
                        count+=set->cTileTemplates().size();
                        continue;
                    } else {
                        id += count;
                        break;
                    }
                }
                if (id==-1) {
                    qDebug() << "Dependent tileTemplateSet not found when saving tilemap.";
                    return -1;
                }

                attr.setValue(QString::number(id));
                element.setAttributeNode(attr);
                root.appendChild(element);
            }
        }
    }
    doc.save(out, 4);
    file.close();
    return 0;
}

int XMLTool::saveTileTemplateSet(TileTemplateSet *templateSet){
    const QList<SharedTileTemplate> templatelist = templateSet->cTileTemplates();
    QString templateSetPath = templateSet->savePath();
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
    attr = doc.createAttribute("Name");
    attr.setValue(templateSet->name());
    root.setAttributeNode(attr);
    doc.appendChild(root);
    for(SharedTileTemplate temp: templatelist){
        element = doc.createElement("TileTemplate");
        attr = doc.createAttribute("Name");
        attr.setValue(temp->name());
        element.setAttributeNode(attr);
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
    file.close();
    return 0;
}
