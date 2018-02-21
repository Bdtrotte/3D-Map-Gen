#ifndef TILEMATERIAL_H
#define TILEMATERIAL_H

#include "imageandsource.h"

#include <QObject>

class TileMaterial : public QObject
{

    Q_OBJECT

public:

    /**
     * @brief Creates a TileMaterial with a default material.
     */
    TileMaterial(QObject *parent = nullptr);

    /**
     * @brief Creates a TileMaterial with the given material.
     */
    TileMaterial(QString name,
                 SharedImageAndSource texture,
                 float ambient,
                 float diffuse,
                 float specular,
                 float shininess,
                 QObject *parent = nullptr);

    QString name() const;
    SharedImageAndSource texture() const;
    float ambient() const;
    float diffuse() const;
    float specular() const;
    float shininess() const;


    void setName(QString name);
    void setTexture(SharedImageAndSource texture);
    void setAmbient(float ambient);
    void setDiffuse(float diffuse);
    void setSpecular(float specular);
    void setShininess(float shininess);

    static TileMaterial *getDefaultMaterial();

signals:
    void nameChanged(QString newName);
    void textureChanged();
    void phongParamsChanged();

private:
    QString mName;

    SharedImageAndSource mTexture;
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;


    /* Singleton for the default material. */
    static TileMaterial *defaultMaterial;

    /* Singleton for the default texture. */
    static SharedImageAndSource getDefaultTexture();
    static SharedImageAndSource DefaultTexture;
};

#endif // TILEMATERIAL_H
