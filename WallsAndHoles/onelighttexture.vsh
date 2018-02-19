#version 150

uniform mat4 mvp;

in vec3 vPosition;
in vec3 vNormal;


in float vReflSpecular;
in float vReflDiffuse;
in float vReflAmbient;
in float vShininess;


in vec2 vTexCoords;



smooth out vec3 fPosition;
smooth out vec3 fNormal;

// NOTE: Interpolating these values smoothly might not make sense.
// Remember to change the interpolation qualifiers in the fragment shader too.
out MaterialBlock {
    smooth float reflSpecular;
    smooth float reflDiffuse;
    smooth float reflAmbient;
    smooth float shininess;
} fMaterial;

out vec2 fTexCoords;


void main(void)
{
    gl_Position = mvp * vec4(vPosition, 1);

    fPosition = vPosition;
    fNormal = vNormal;

    fMaterial.reflSpecular = vReflSpecular;
    fMaterial.reflDiffuse = vReflDiffuse;
    fMaterial.reflAmbient = vReflAmbient;
    fMaterial.shininess = vShininess;

    fTexCoords = vTexCoords;
}
