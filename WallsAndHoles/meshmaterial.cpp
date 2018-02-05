#include "meshmaterial.h"

MeshMaterial::MeshMaterial(float ambient, float diffuse, float specular, float shininess)
    : mKind(TextureLess),
      mReflAmbient(ambient),
      mReflDiffuse(diffuse),
      mReflSpecular(specular),
      mShininess(shininess)
{

}


MeshMaterial::MeshMaterial(float ambient, float diffuse, float specular, float shininess, QOpenGLTexture *texture)
    : mKind(SingleTexture),
      mReflAmbient(ambient),
      mReflDiffuse(diffuse),
      mReflSpecular(specular),
      mShininess(shininess)
{
    mTextures.push_back(texture);
}


float MeshMaterial::getReflAmbient() const { return mReflAmbient; }
float MeshMaterial::getReflDiffuse() const { return mReflDiffuse; }
float MeshMaterial::getReflSpecular() const { return mReflSpecular; }
float MeshMaterial::getShininess() const { return mShininess; }

MeshMaterial::Kind MeshMaterial::getKind() const { return mKind; }

QOpenGLTexture *MeshMaterial::getTexture(int idx) const { return mTextures[idx]; }

int MeshMaterial::getNumTextures() const { return mTextures.size(); }

const MeshMaterial MeshMaterial::Soft = MeshMaterial(1, 1, 1, 1);
const MeshMaterial MeshMaterial::Metal = MeshMaterial(1, 0.5, 2, 5);
