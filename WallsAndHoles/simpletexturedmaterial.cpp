#include "simpletexturedmaterial.h"


SimpleTexturedMaterial::SimpleTexturedMaterial(QSharedPointer<QImage> texture, float ambient, float diffuse, float specular, float shininess)
    : PhongMaterial(ambient, diffuse, specular, shininess)
    , mTexture(texture) {}

QSharedPointer<QImage> SimpleTexturedMaterial::getTexture() const { return mTexture; }
