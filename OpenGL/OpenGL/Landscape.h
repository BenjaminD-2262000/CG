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
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	VAO m_VAO;
	Texture m_texture = Texture("recourses/Textures/rocks.jpg", "diffuse", 0);
	Texture m_specular = Texture("recourses/Textures/rocks_spec.jpg", "specular", 1);

	void generateVertices();
	void generateIndices();
	void setupMesh();

public:
	Landscape(const char* heightmap, int rezolution);
	void drawTerrain(Shader& shader, Camera& camera);
	float getHeight(float x, float z);
};

