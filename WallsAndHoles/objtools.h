#ifndef OBJTOOLS_H
#define OBJTOOLS_H
#include <QFile>
#include <QTextStream>
#include <cassert>
#include <QMap>
#include <QVector3D>
#include <QDebug>

#include "imageandsource.h"
#include "simpletexturedobject.h"

typedef QSharedPointer<SimpleTexturedObject> SharedSimpleTexturedObject;

class Material
{
public:
    Material(QString _name, QVector3D _Ka, QVector3D _Kd, QVector3D _Ks, int _Ns, int _illum, QString KaImage, QString KdImage)
        :name(_name), Ka(_Ka), Kd(_Kd), Ks(_Ks), Ns(_Ns), illum(_illum), Map_Ka(KaImage), Map_Kd(KdImage)
    {
    }
    // Material Name
    QString name;
    // Ambient Color
    QVector3D Ka;
    // Diffuse Color
    QVector3D Kd;
    // Specular Color
    QVector3D Ks;
    // Specular Exponent
    float Ns;
    // Illumination
    int illum;
    // Name of Ambient Texture Map
    QString Map_Ka;
    // Name of Diffuse Texture Map
    QString Map_Kd;

    /**
     * @brief Serialize the material object into a string
     * return a QString that can be directly write into .mtl
     */
    QString serialize();
};

typedef QSharedPointer<Material> SharedMaterial;

class OBJModel
{
public:
    OBJModel(QString _name="map"){
        name = _name;
        mSaveDirectory = '.';
    }

    QString name;
    void addSimpleTextured(SharedSimpleTexturedObject object);
    void saveOBJ(QString path);
    void saveMTL(QString path);
    void saveImages(QString path);
    void save(QString path);
    void setSaveDirectory(QString path){ mSaveDirectory = path; }

private:
    QVector<SharedSimpleTexturedObject> mObjects;
    QMap<QString, SharedMaterial> mMaterials;
    QMap<QString, QSharedPointer<QImage>> mImages;
    QString mSaveDirectory;
};

typedef QSharedPointer<OBJModel> SharedOBJModel;

#endif // OBJTOOLS_H
