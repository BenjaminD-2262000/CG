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

	void generateVertices();
	void generateIndices();
	void setupMesh();

public:
	Landscape(const char* heightmap, int rezolution);
	void drawTerrain(Shader& shader, Camera& camera);

};

