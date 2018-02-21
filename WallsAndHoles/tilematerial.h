#ifndef TILEMATERIAL_H
#define TILEMATERIAL_H

#include <QObject>
#include <QSharedPointer>
#include <QImage>


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
                 QSharedPointer<QImage> texture,
                 float ambient,
                 float diffuse,
                 float specular,
                 float shininess,
                 QObject *parent = nullptr);

    QString name() const;
    QSharedPointer<QImage> texture() const;
    float ambient() const;
    float diffuse() const;
    float specular() const;
    float shininess() const;


    void setName(QString name);
    void setTexture(QSharedPointer<QImage> texture);
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

    QSharedPointer<QImage> mTexture;
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;


    /* Singleton for the default material. */
    static TileMaterial *defaultMaterial;

    /* Singleton for the default texture. */
    static QSharedPointer<QImage> getDefaultTexture();
    static QSharedPointer<QImage> DefaultTexture;
};

#endif // TILEMATERIAL_H
