#version 150

uniform mat4 mvp;

in vec3 vPosition;
in vec3 vNormal;

in MaterialBlock {
    float reflSpecular;
    float reflDiffuse;
    float reflAmbient;
    float shininess;
} vMaterial;



smooth out vec3 fPosition;
smooth out vec3 fNormal;

out MaterialBlock {
    float reflSpecular;
    float reflDiffuse;
    float reflAmbient;
    float shininess;
} fMaterial;

void main(void)
{
    gl_Position = mvp * vec4(vPosition, 1);

    fPosition = vPosition;
    fNormal = vNormal;
    fMaterial = vMaterial;
}
