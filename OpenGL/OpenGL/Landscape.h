#pragma once

#include <stb/stb_image.h>
#include <vector>
#include "EBO.h"
#include "VAO.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"


class Landscape
{
private:
	int m_width, m_height, m_nChannels, m_rezolution;
	unsigned char* m_data;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	VAO m_VAO;
	Texture m_textureGround = Texture("recourses/Textures/mud.jpg", "diffuse", 0);
	Texture m_specularGround = Texture("recourses/Textures/mud_spec.jpg", "specular", 0);
	Texture m_textureMountain = Texture("recourses/Textures/mountain.jpg", "diffuse", 1);
	Texture m_specularMountain = Texture("recourses/Textures/mountain_spec.jpg", "specular", 1);

	void generateVertices();
	void calculateNormals();
	void generateIndices();
	void setupMesh();

public:
	Landscape(const char* heightmap, int rezolution);
	void drawTerrain(Shader& shader, Camera& camera);
	float getHeight(float x, float z);
};

