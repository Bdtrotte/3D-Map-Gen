#ifndef MESHMATERIAL_H
#define MESHMATERIAL_H

#include <QOpenGLTexture>

/**
 * @brief The MeshMaterial class represents the "material" information for a mesh.
 *
 * The MeshMaterial class contains the information needed to apply the Phong lighting model.
 * EVENTUALLY, this class will likely include a texture index and texture coordinates.
 */
class MeshMaterial {
public:


    enum Kind {
        TextureLess, SingleTexture
    };


    /**
     * @brief Creates a texture-less material.
     * @param ambient       Ambient reflection coefficient (Phong).
     * @param diffuse       Diffuse reflection coefficient (Phong).
     * @param specular      Specular reflection coefficient (Phong).
     * @param shininess     Shininess coefficient (Phong).
     */
    MeshMaterial(float ambient, float diffuse, float specular, float shininess);

    /**
     * @brief Creates a single-texture material.
     * @param ambient       Ambient reflection coefficient (Phong).
     * @param diffuse       Diffuse reflection coefficient (Phong).
     * @param specular      Specular reflection coefficient (Phong).
     * @param shininess     Shininess coefficient (Phong).
     * @param texture       The texture.
     */
    MeshMaterial(float ambient, float diffuse, float specular, float shininess, QOpenGLTexture *texture);

    float getReflAmbient() const;
    float getReflDiffuse() const;
    float getReflSpecular() const;
    float getShininess() const;

    Kind getKind() const;

    /**
     * @brief Returns the texture with the given index. Assumes index is within bounds.
     *
     * The meaning of the index depends on the implemenation.
     *
     * @param idx   The index of the texture for this material.
     * @return      The texture at that index.
     */
    QOpenGLTexture *getTexture(int idx = 0) const;

    int getNumTextures() const;

    static const MeshMaterial Metal;
    static const MeshMaterial Soft;

protected:

    Kind mKind;

    // Phong shader ambient / diffuse / specular / shininess.
    float mReflAmbient;
    float mReflDiffuse;
    float mReflSpecular;
    float mShininess;


    /**
     * @brief A list of texture pointers. Usage depends on material kind.
     */
    QVector<QOpenGLTexture *> mTextures;
};

#endif // MESHMATERIAL_H
