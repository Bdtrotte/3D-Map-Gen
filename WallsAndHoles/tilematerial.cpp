#include "tilematerial.h"

TileMaterial::TileMaterial(QObject *parent)
    : TileMaterial(getDefaultTexture(), 1, 1, 1, 1, parent)
{
}


TileMaterial::TileMaterial(QSharedPointer<QImage> texture, float ambient, float diffuse, float specular, float shininess, QObject *parent)
    : QObject(parent)
    , mTexture(texture)
    , mAmbient(ambient)
    , mDiffuse(diffuse)
    , mSpecular(specular)
    , mShininess(shininess) {}



QSharedPointer<QImage> TileMaterial::getTexture() const { return mTexture; }
float TileMaterial::getAmbient() const { return mAmbient; }
float TileMaterial::getDiffuse() const { return mDiffuse; }
float TileMaterial::getSpecular() const { return mSpecular; }
float TileMaterial::getShininess() const { return mShininess; }


void TileMaterial::setTexture(QSharedPointer<QImage> texture) { mTexture = texture; emit textureChanged(); }
void TileMaterial::setAmbient(float ambient) { mAmbient = ambient; emit phongParamsChanged(); }
void TileMaterial::setDiffuse(float diffuse) { mDiffuse = diffuse; emit phongParamsChanged(); }
void TileMaterial::setSpecular(float specular) { mSpecular = specular; emit phongParamsChanged(); }
void TileMaterial::setShininess(float shininess) { mShininess = shininess; emit phongParamsChanged(); }


TileMaterial *TileMaterial::defaultMaterial = nullptr;
TileMaterial *TileMaterial::getDefaultMaterial()
{
    if (defaultMaterial == nullptr)
        defaultMaterial = new TileMaterial(getDefaultTexture(), 1, 1, 1, 1, nullptr);

    return defaultMaterial;
}


QSharedPointer<QImage> TileMaterial::DefaultTexture = nullptr;
QSharedPointer<QImage> TileMaterial::getDefaultTexture()
{
    if (DefaultTexture == nullptr)
        DefaultTexture = QSharedPointer<QImage>::create(":/textures/exampleTexture2.png");

    return DefaultTexture;
}
