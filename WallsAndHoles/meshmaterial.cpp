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


const MeshMaterial MeshMaterial::Soft = MeshMaterial(1, 1, 1, 1);
const MeshMaterial MeshMaterial::Metal = MeshMaterial(1, 0.5, 2, 5);
