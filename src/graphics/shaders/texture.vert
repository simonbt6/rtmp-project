#version 330 core


layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * position;
    v_TexCoord = vec2(1, 1) - texCoord;
}