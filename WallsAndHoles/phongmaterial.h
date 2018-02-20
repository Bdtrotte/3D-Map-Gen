#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

/**
 * @brief The PhongMaterial class simply contains the information needed for
 * the Phong reflection model.
 */
class PhongMaterial
{
public:
    PhongMaterial(float ambient, float diffuse, float specular, float shininess);

    float getAmbient() const;
    float getDiffuse() const;
    float getSpecular() const;
    float getShininess() const;

private:

    // Phong reflection model parameters.
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;
};

#endif // PHONGMATERIAL_H
