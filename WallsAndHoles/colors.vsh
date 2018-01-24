#version 150

uniform mat4 mvp;

in vec3 vPos;
in vec4 vColor;
out vec4 fColor;

void main(void)
{
    gl_Position = mvp * vec4(vPos, 1);
    fColor = vColor;
}
