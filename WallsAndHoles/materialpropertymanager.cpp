#include "materialpropertymanager.h"

#include "propertybrowser.h"
#include "imagefinderbar.h"

MaterialPropertyManager::MaterialPropertyManager(TileMaterial *material)
    : mMaterial(material)
{
    connect(mMaterial, &TileMaterial::nameChanged,
            this, [this](QString name) {
        emit propertyChanged("Name", name);
    });
}

void MaterialPropertyManager::propertyEdited(QString propertyName, QVariant value)
{
    if (propertyName == "Name") {
        mMaterial->setName(value.toString());
    } else if (propertyName == "Ambient") {
        mMaterial->setAmbient(value.toFloat());
    } else if (propertyName == "Diffuse") {
        mMaterial->setDiffuse(value.toFloat());
    } else if (propertyName == "Specular") {
        mMaterial->setSpecular(value.toFloat());
    } else if (propertyName == "Shininess") {
        mMaterial->setShininess(value.toFloat());
    }
}

QVector<QVector<QVariant> > MaterialPropertyManager::properties()
{
    ImageFinderBar *imageFindBar = new ImageFinderBar(mMaterial->texture()->source());

    connect(imageFindBar, &ImageFinderBar::imageChanged,
            this, &MaterialPropertyManager::textureChanged);

    return {
        {"Name", mMaterial->name(), true},
        {"Ambient", mMaterial->ambient(), true, 0, 1},
        {"Diffuse", mMaterial->diffuse(), true, 0, 1},
        {"Specular", mMaterial->specular(), true, 0, 1},
        {"Shininess", mMaterial->shininess(), true, 0, 1},
        {"Texture", QVariant::fromValue<ImageFinderBar *>(imageFindBar), true, PropertyBrowser::LineWidget}
    };
}

void MaterialPropertyManager::textureChanged(SharedImageAndSource imageAndSource)
{
    mMaterial->setTexture(imageAndSource);
}
