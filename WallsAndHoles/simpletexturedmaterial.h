#ifndef SIMPLETEXTUREDMATERIAL_H
#define SIMPLETEXTUREDMATERIAL_H

#include <QSharedPointer>
#include <QImage>

#include "phongmaterial.h"

/**
 * @brief The SimpleTexturedMaterial class extends the PhongMaterial class
 * by adding a single texture. This contains all the material data needed
 * to define a SimpleTexturedObject.
 *
 * The texture is stored as a shared pointer to a QImage. This avoids
 * wasting resources, as textures are heavily reused.
 *
 * The getTexture() method returns the shared pointer because the texture
 * is meant to be passed to the M2M system (otherwise, it would return
 * a const reference).
 */
class SimpleTexturedMaterial : public PhongMaterial
{
public:
    SimpleTexturedMaterial(QSharedPointer<QImage> texture, float ambient, float diffuse, float specular, float shininess);

    QSharedPointer<QImage> getTexture() const;

private:
    QSharedPointer<QImage> mTexture;
};

#endif // SIMPLETEXTUREDMATERIAL_H
