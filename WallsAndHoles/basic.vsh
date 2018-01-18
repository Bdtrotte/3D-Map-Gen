#version 150

in vec3 qt_Vertex;
//in vec4 qt_Color;
uniform mat4 qt_ModelViewProjectionMatrix;

out vec4 color;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * vec4(qt_Vertex, 1);

    color = vec4(1, 0, 1, 1);
}
