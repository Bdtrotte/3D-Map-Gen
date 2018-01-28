#version 150

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;

uniform vec3 uAmbientColor;
uniform vec3 uSourceSpecularColor;
uniform vec3 uSourceDiffuseColor;



smooth in vec3 fPosition; // the world-space position of the fragment
smooth in vec3 fNormal;


in MaterialBlock
{
    smooth float reflSpecular;
    smooth float reflDiffuse;
    smooth float reflAmbient;
    smooth float shininess;
} fMaterial;


out vec4 fragColor;

void main(void)
{
    vec3 normal = normalize(fNormal);

    vec3 pointToLight = normalize(uLightPosition - fPosition);
    vec3 pointToCamera = normalize(uCameraPosition - fPosition);
    float dotDiffuse = max(dot(pointToLight, normal), 0);

    vec3 reflectedRay = 2 * dot(pointToLight, normal) * normal - pointToLight;
    float dotSpecular = max(dot(reflectedRay, pointToCamera), 0);


    vec3 illumination = fMaterial.reflAmbient * uAmbientColor
                        + fMaterial.reflDiffuse * uSourceSpecularColor * dotDiffuse
                        + fMaterial.reflSpecular * uSourceSpecularColor * pow(dotSpecular, fMaterial.shininess);
    vec3 baseColor = vec3(1, 1, 1);

    fragColor = vec4(illumination * baseColor, 1);
}
