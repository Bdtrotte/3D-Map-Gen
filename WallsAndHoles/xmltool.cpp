#include "xmltool.h"

#include "tiletemplatesetsmanager.h"
#include "tilematerialset.h"

#include <QDebug>
#include <QMessageBox>

QDomElement tileMapElement(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets, QDomDocument &doc);
QDomElement tileTemplateSetElement(TileTemplateSet *templateSet, QDomDocument &doc);
QDomElement tileMaterialElement(TileMaterial *material, QDomDocument &doc);

QDomElement tileMapElement(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets, QDomDocument &doc)
{
    QDomElement root = doc.createElement( "TileMap" );
    root.setAttribute("width", tileMap->mapSize().width());
    root.setAttribute("height", tileMap->mapSize().height());
    root.setAttribute("isIndoor", tileMap->isIndoor());
    root.setAttribute("hasCeiling", tileMap->hasCeiling());

    QVector<SavableTileTemplateSet *> usedTileTemplateSets;
    for (SavableTileTemplateSet *tts : tileTemplateSets) {
        if (tileMap->isTileTemplateSetUsed(tts)) {
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

    return root;
}

QDomElement tileTemplateSetElement(TileTemplateSet *templateSet, QDomDocument &doc)
{
    QDomElement root = doc.createElement("TileTemplateSet");
    root.setAttribute("name", templateSet->name());

    const QList<TileTemplate *> templateList = templateSet->cTileTemplates();

    QList<TileMaterial *> usedMaterials;
    for (TileTemplate *t : templateList) {
        TileMaterial *m = t->topMaterial();
        if (m != TileMaterial::getDefaultMaterial() && usedMaterials.indexOf(m) == -1)
            usedMaterials.append(m);
        if (t->hasSideMaterial()) {
            m = t->sideMaterial();
            if (m != TileMaterial::getDefaultMaterial() && usedMaterials.indexOf(m) == -1)
                usedMaterials.append(m);
        }
    }

    for (TileMaterial *m : usedMaterials)
        root.appendChild(tileMaterialElement(m, doc));

    for (TileTemplate *t: templateList) {
        QDomElement templateE = doc.createElement("TileTemplate");

        templateE.setAttribute("name", t->name());
        templateE.setAttribute("height", t->height());
        templateE.setAttribute("thickness", t->thickness());
        templateE.setAttribute("bridge", t->bridgeTiles());
        templateE.setAttribute("diagonal", t->connectDiagonals());
        templateE.setAttribute("position", QString("%1,%2").arg(
                               QString::number(t->position()[0]),
                               QString::number(t->position()[1])));
        templateE.setAttribute("color", t->color().name());
        templateE.setAttribute("hassidemtrl", t->hasSideMaterial());

        templateE.setAttribute("topmaterial", usedMaterials.indexOf(t->topMaterial()));
        if (t->hasSideMaterial())
            templateE.setAttribute("sidematerial", usedMaterials.indexOf(t->sideMaterial()));
        else
            templateE.setAttribute("sidematerial", -1);

        root.appendChild(templateE);
    }

    return root;
}

QDomElement tileMaterialElement(TileMaterial *material, QDomDocument &doc)
{
    QDomElement root = doc.createElement("TileMaterial");

    root.setAttribute("name", material->name());
    root.setAttribute("texture", material->texture()->source());
    root.setAttribute("ambient", material->ambient());
    root.setAttribute("diffuse", material->diffuse());
    root.setAttribute("specular", material->specular());
    root.setAttribute("shininess", material->shininess());

    return root;
}

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
                bool isIndoor = false;
                bool hasCeiling = false;
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name() == "width")
                         size.setWidth(attr.value().toInt());
                    if (attr.name() == "height")
                         size.setHeight(attr.value().toInt());
                    if (attr.name() == "isIndoor")
                        isIndoor = attr.value().toInt();
                    if (attr.name() == "hasCeiling")
                        hasCeiling = attr.value().toInt();
                }

                tileMap = new TileMap(size, isIndoor, hasCeiling);
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
    QList<TileMaterial *> materials;
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
            } else if (xmlReader.name() == "TileMaterial") {
                QString name = "";
                QString texturePath = "";
                float ambient = 1;
                float diffuse = 1;
                float specular = 1;
                float shininess = 1;

                for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
                    if (attr.name() == "name")
                        name = attr.value().toString();
                    else if (attr.name() == "texture")
                        texturePath = attr.value().toString();
                    else if (attr.name() == "ambient")
                        ambient = attr.value().toFloat();
                    else if (attr.name() == "diffuse")
                        diffuse = attr.value().toFloat();
                    else if (attr.name() == "specular")
                        specular = attr.value().toFloat();
                    else if (attr.name() == "shininess")
                        shininess = attr.value().toFloat();
                }

                TileMaterial *m = new TileMaterial(name, ImageAndSource::getSharedImageAndSource(texturePath), ambient, diffuse, specular, shininess);

                if (TileMaterial *tm = TileMaterialSet::getInstance()->materialInSet(*m))
                    m = tm;
                else
                    TileMaterialSet::getInstance()->addMaterial(m);

                materials.append(m);
            } else if (xmlReader.name() == "TileTemplate") {
                QString name;
                float thickness;
                float height;
                QVector2D position;
                QColor color;
                int topMaterialId;
                int sideMaterialId;
                bool hasSideMaterial;
                bool bridge = false;
                bool diagonal = false;
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
                    else if (attr.name() == "topmaterial")
                        topMaterialId = attr.value().toInt();
                    else if (attr.name() == "sidematerial")
                        sideMaterialId = attr.value().toInt();
                    else if (attr.name() == "hassidemtrl")
                        hasSideMaterial = attr.value().toInt();
                    else if (attr.name() == "bridge")
                        bridge = attr.value().toInt();
                    else if (attr.name() == "diagonal")
                        diagonal = attr.value().toInt();
                }

                TileMaterial *topM;
                if (topMaterialId == -1)
                    topM = TileMaterial::getDefaultMaterial();
                else
                    topM = materials[topMaterialId];

                TileTemplate *tileTemplate = new TileTemplate(color, name, height, thickness, topM, position, bridge, diagonal, templateSet);

                tileTemplate->setHasSideMaterial(hasSideMaterial);
                if (sideMaterialId != -1)
                    tileTemplate->setSideMaterial(materials[sideMaterialId]);

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

int XMLTool::saveTileMap(TileMap *tileMap, const QList<SavableTileTemplateSet *> &tileTemplateSets)
{
    QString tileMapPath = tileMap->savePath();
    QFile file(tileMapPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return OpenFileError;

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction( "xml", "version = \'1.0\' encoding=\'UTF-8\'" );
    doc.appendChild(instruction);
    doc.appendChild(tileMapElement(tileMap, tileTemplateSets, doc));

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
