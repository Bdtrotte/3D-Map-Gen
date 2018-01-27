#ifndef MESHMATERIAL_H
#define MESHMATERIAL_H


/**
 * @brief The MeshMaterial class represents the "material" information for a mesh.
 *
 * The MeshMaterial class contains the information needed to apply the Phong lighting model.
 * EVENTUALLY, this class will likely include a texture index and texture coordinates.
 */
class MeshMaterial {
public:
    MeshMaterial(float ambient, float diffuse, float specular, float shininess);

    float getReflAmbient() const;
    float getReflDiffuse() const;
    float getReflSpecular() const;
    float getShininess() const;

protected:
    float mReflAmbient;
    float mReflDiffuse;
    float mReflSpecular;
    float mShininess;
};

#endif // MESHMATERIAL_H
