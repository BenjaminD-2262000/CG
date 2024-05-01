#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

// Outputs
out float Height;
out vec3 Position;
out vec3 v_Color;
out vec2 texCoord;
out vec3 Normal;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
	Height = aPos.y;
	Position = (camMatrix * model * vec4(aPos, 1.0)).xyz;
	v_Color = vec3(1.0f,1.0f,1.0f);
	gl_Position =  model * camMatrix * vec4(aPos, 1.0f);
	texCoord = aTex;
	Normal = aNormal;
}