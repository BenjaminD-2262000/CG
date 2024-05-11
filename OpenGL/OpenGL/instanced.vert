#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;
// Instace Matrix
layout (location = 4) in mat4 instanceMatrix;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

// Outputs the rotation matrix for the Fragment Shader
out mat4 aRotation;

// Outputs the player position for the Fragment Shader
out vec3 playerPos;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;

uniform vec3 location;

void main()
{
    // Calculates current position
    crntPos = vec3(instanceMatrix * vec4(aPos, 1.0f));	
    Normal = aNormal;
    // Assigns the colors from the Vertex Data to "color"
    color = aColor;
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    
    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * vec4(crntPos, 1.0);

    playerPos = vec3(camMatrix * vec4(crntPos, 1.0));

}