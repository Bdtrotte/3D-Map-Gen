#include "xmltool.h"

#include "tiletemplatesetsmanager.h"

#include <QDebug>
#include <QMessageBox>

TileMap *XMLTool::openTileMap(QString tileMapPath, TileTemplateSetsManager *tileTemplateSetsManager)
{
    //load the file
    QFile file(tileMapPath);
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text))
        return nullptr;

    QXmlStreamReader xmlReader(file.readAll());
    TileMap *tileMap;

    QVector<SavableTileTemplateSet *> loadedTileTemplateSets;

    //Parse the XML until we reach end of it
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        //If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument)
            continue;

        //If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "TileMap") {
                QSize size;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name() == "width")
                         size.setWidth(attr.value().toInt());
                    if (attr.name() == "height")
                         size.setHeight(attr.value().toInt());
                }
                tileMap = new TileMap(size);
                tileMap->setSavePath(tileMapPath);
            } else if (xmlReader.name() == "TileTemplateSet") {
                QString path = xmlReader.attributes().first().value().toString();

                SavableTileTemplateSet *templateSet = tileTemplateSetsManager->loadTileTemplateSet(path, true);

                if (templateSet == nullptr)
                    return nullptr;

                loadedTileTemplateSets.append(templateSet);
            } else if (xmlReader.name() == "Tile") {
                float x;
                float y;
                int setId = -2, templateId;
                float relativeThickness;
                float relativeHeight;
                QVector2D relativePosition;

                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name() == "x") {
                        x = attr.value().toInt();
                    } else if (attr.name() == "y") {
                        y = attr.value().toInt();
                    } else if (attr.name() == "templateSetId") {
                        setId = attr.value().toInt();
                    } else if (attr.name() == "templateId") {
                        templateId = attr.value().toInt();
                    } else if (attr.name() == "relativeThickness") {
                        relativeThickness = attr.value().toFloat();
                    } else if (attr.name() == "relativeHeight") {
                        relativeHeight = attr.value().toFloat();
                    } else if (attr.name() == "relativePosition") {
                        QStringList pos = attr.value().toString().split(',');
                        relativePosition[0] = pos[0].toFloat();
                        relativePosition[1] = pos[1].toFloat();
                    }
                }

                TileTemplate *tileTemplate;

                if (setId == -1) {
                    tileTemplate = tileMap->defaultTileTemplateSet()->tileTemplateAt(templateId);
                } else if (setId == -2
                           || setId >= loadedTileTemplateSets.size()
                           || templateId >= loadedTileTemplateSets[setId]->size()) {
                    return nullptr;
                } else {
                    tileTemplate = loadedTileTemplateSets[setId]->tileTemplateAt(templateId);
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

SavableTileTemplateSet *XMLTool::openTileTemplateSet(QString templateSetPath)
{
    //load the file
    QFile file(templateSetPath);
    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text))
        return nullptr;

    QXmlStreamReader xmlReader(file.readAll());
    SavableTileTemplateSet *templateSet;
    //Parse the XML until we reach end of it
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        //If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument)
            continue;

        //If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "TileTemplateSet") {
                templateSet = new SavableTileTemplateSet(templateSetPath,
                                                         xmlReader.attributes()[0].value().toString(),
                                                         true);
            } else if (xmlReader.name() == "TileTemplate") {
                QString name;
                float thickness;
                float height;
                QVector2D position;
                QColor color;
                foreach (const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name() == "name")
                        name = attr.value().toString();
                    else if (attr.name() == "thickness")
                        thickness = attr.value().toFloat();
                    else if (attr.name() == "height")
                        height = attr.value().toFloat();
                    else if (attr.name() == "position") {
                        QStringList pos = attr.value().toString().split(',');
                        position[0] = pos[0].toFloat();
                        position[1] = pos[1].toFloat();
                    } else if (attr.name() == "color")
                        color = QColor(attr.value().toString());
                }

                TileTemplate *tileTemplate = new TileTemplate(color, name, height, thickness, position, templateSet);
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

QDomElement tileTemplateSetElement(TileTemplateSet *templateSet, QDomDocument &doc)
{
    QDomElement root = doc.createElement("TileTemplateSet");
    root.setAttribute("name", templateSet->name());

    const QList<TileTemplate *> templateList = templateSet->cTileTemplates();
    for (TileTemplate *t: templateList) {
        QDomElement templateE = doc.createElement("TileTemplate");

        templateE.setAttribute("name", t->name());
        templateE.setAttribute("height", t->height());
        templateE.setAttribute("thickness", t->thickness());
        templateE.setAttribute("position", QString("%1,%2").arg(
                               QString::number(t->position()[0]),
                               QString::number(t->position()[1])));
        templateE.setAttribute("color", t->color().name());

        root.appendChild(templateE);
    }

    return root;
}

int XMLTool::saveTileMap(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets)
{
    QString tileMapPath = tileMap->savePath();
    QFile file(tileMapPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return OpenFileError;

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    doc.appendChild(instruction);

    QDomElement root = doc.createElement( "TileMap" );
    root.setAttribute("width", tileMap->mapSize().width());
    root.setAttribute("height", tileMap->mapSize().height());

    QVector<SavableTileTemplateSet *> usedTileTemplateSets;
    for (SavableTileTemplateSet *tts : tileTemplateSets) {
        if (tileMap->tileTemplateSetUsed(tts)) {
            usedTileTemplateSets.append(tts);

            QDomElement templateSet = doc.createElement("TileTemplateSet");
            templateSet.setAttribute("savePath", tts->savePath());
            root.appendChild(templateSet);
        }
    }

    for (int y = 0; y < tileMap->mapSize().height(); ++y) {
        for (int x = 0; x < tileMap->mapSize().width(); ++x) {
            const Tile &tile = tileMap->cTileAt(x, y);

            if(tile.hasTileTemplate()) {
                QDomElement tileE = doc.createElement("Tile");

                tileE.setAttribute("x", x);
                tileE.setAttribute("y", y);
                tileE.setAttribute("relativeThickness", tile.relativeThickness());
                tileE.setAttribute("relativeHeight", tile.relativeHeight());
                tileE.setAttribute("relativePosition", QString("%1,%2").arg(
                                       QString::number(tile.relativePosition()[0]),
                                       QString::number(tile.relativePosition()[1])));

                int setId = -2, templateId;
                int id = tileMap->defaultTileTemplateSet()->cTileTemplates().indexOf(tile.tileTemplate());
                if (id != -1) {
                    setId = -1, templateId = id;
                } else {
                    for (int i = 0; i < usedTileTemplateSets.size(); ++i) {
                        int j = usedTileTemplateSets[i]->cTileTemplates().indexOf(tile.tileTemplate());
                        if (j != -1) {
                            setId = i;
                            templateId = j;
                            break;
                        }
                    }
                }

                //There should be no case where a tile is assigned to a template that is not contained in any active set.
                // TODO : This currently does not consider default templates
                Q_ASSERT(setId != -2);

                tileE.setAttribute("templateSetId", setId);
                tileE.setAttribute("templateId", templateId);

                root.appendChild(tileE);
            }
        }
    }

    doc.appendChild(root);

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
    return NoError;
}

int XMLTool::saveTileTemplateSet(SavableTileTemplateSet *templateSet)
{
    QFile file(templateSet->savePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return OpenFileError;

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    doc.appendChild(instruction);

    doc.appendChild(tileTemplateSetElement(templateSet, doc));

    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
    return NoError;
}
