#version 330 core

in vec3 v_Color;
in float Height;

out vec4 color;
void main()
{
    float h = (Height + 16)/32.0f;
    color = vec4(h, h, h, 1.0);
}