#version 330 core

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D u_Texture;

void main()
{
    outColor = texture(u_Texture, Texcoord);
}