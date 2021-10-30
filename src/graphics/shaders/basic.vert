#version 450

in vec2 position;


out vec4 vertexColor;
uniform vec4 u_Color;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    vertexColor = u_Color;
}