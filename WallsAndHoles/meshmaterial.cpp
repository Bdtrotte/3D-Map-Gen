#include "meshmaterial.h"

MeshMaterial::MeshMaterial(float ambient, float diffuse, float specular, float shininess)
    : mReflAmbient(ambient),
      mReflDiffuse(diffuse),
      mReflSpecular(specular),
      mShininess(shininess)
{

}


float MeshMaterial::getReflAmbient() const { return mReflAmbient; }
float MeshMaterial::getReflDiffuse() const { return mReflDiffuse; }
float MeshMaterial::getReflSpecular() const { return mReflSpecular; }
float MeshMaterial::getShininess() const { return mShininess; }
