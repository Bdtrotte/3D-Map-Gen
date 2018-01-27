#version 150

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;

uniform vec3 uAmbientColor;
uniform vec3 uSourceSpecularColor;
uniform vec3 uSourceDiffuseColor;



in vec3 fPosition; // the world-space position of the fragment
in vec3 fNormal;


in MaterialBlock
{
    float reflSpecular;
    float reflDiffuse;
    float reflAmbient;
    float shininess;
} fMaterial;


out vec4 fragColor;

void main(void)
{
    fNormal = normalize(fNormal);

    vec3 pointToLight = normalize(uLightPosition - fPosition);
    vec3 pointToCamera = normalize(uCameraPosition - fPosition);
    float dotDiffuse = max(dot(pointToLight, fNormal), 0);

    vec3 reflectedRay = 2 * dot(pointToLight, fNormal) * fNormal - pointToLight;
    float dotSpecular = max(dot(reflectedRay, pointToCamera));


    vec3 illumination = fMaterial.reflAmbient * uAmbientColor
                        + fMaterial.reflDiffuse * uSourceSpecularColor * dotDiffuse
                        + fMaterial.reflSpecular * uSourceSpecularColor * pow(dotSpecular, fMaterial.shininess);
    vec3 baseColor = vec3(1, 0, 1);

    fragColor = vec4(illumination * baseColor);
}
