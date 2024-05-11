#version 330 core

layout(location = 0) in vec3 aPos;

out vec2 TexCoords;

uniform float scale;

void main()
{
    gl_Position = vec4(aPos * scale, 1.0);
    TexCoords = (aPos.xy + 1.0) * 0.5;
}

