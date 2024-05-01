#include "Landscape.h"

void Landscape::generateVertices()
{
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	float texScale = 100.0f;
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			unsigned char* pixelOffset = m_data + (col + m_width * row) * m_nChannels;
			unsigned char y = pixelOffset[0];

			Vertex vertex;
			float xpos = -m_height / 2.0f + m_height * row / (float)m_height;
			float ypos = (int)(y * yScale - yShift);
			float zpos = -m_width / 2.0f + m_width * col / (float)m_width;

			vertex.Position = glm::vec3(xpos, ypos, zpos);
			vertex.Color = glm::vec3(1.0f, 1.0f, 1.0f);
			vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vertex.TexUV = glm::vec2(col / (float)(m_width - 1) * texScale, row / (float)(m_height - 1) * texScale);
			m_vertices.push_back(vertex);
		}
	}
}

void Landscape::generateIndices()
{
	for (unsigned int row = 0; row < m_height - 1; row += m_rezolution)
	{
		for (unsigned int col = 0; col < m_width; col += m_rezolution)
		{
			for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
			{
				m_indices.push_back(col + m_width * (row + k*m_rezolution));
			}
		}
	}

}

void Landscape::setupMesh()
{
	m_VAO.Bind();
	VBO VBO(m_vertices);
	EBO EBO(m_indices);

	m_VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	m_VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	m_VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

Landscape::Landscape(const char* heightmap, int rezolution)
{
	m_data = stbi_load(heightmap, &m_width, &m_height, &m_nChannels, 0);
	m_rezolution = rezolution;
	generateVertices();
	stbi_image_free(m_data);
	generateIndices();
	setupMesh();
}

void Landscape::drawTerrain(Shader& shader, Camera& camera)
{
	shader.Activate();
	m_VAO.Bind();

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");


	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	int numStrips = (m_height - 1) / m_rezolution;
	int numTrisPerStrip = (m_width / m_rezolution) * 2 - 2;

	//add grass texture
	m_texture.texUnit(shader, "diffuse0", 0);
	m_specular.texUnit(shader, "specular0", 0);
	m_texture.Bind();

	
	for (unsigned int strip = 0; strip < numStrips; ++strip)
	{
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			numTrisPerStrip+2, // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
	}
	
	m_VAO.Unbind();
}


