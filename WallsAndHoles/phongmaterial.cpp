#include "phongmaterial.h"

PhongMaterial::PhongMaterial(float ambient, float diffuse, float specular, float shininess)
    : mAmbient(ambient)
    , mDiffuse(diffuse)
    , mSpecular(specular)
    , mShininess(shininess) {}


float PhongMaterial::getAmbient() const { return mAmbient; }
float PhongMaterial::getDiffuse() const { return mDiffuse; }
float PhongMaterial::getSpecular() const { return mSpecular; }
float PhongMaterial::getShininess() const { return mShininess; }
