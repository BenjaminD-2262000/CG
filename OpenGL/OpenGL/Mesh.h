#pragma once

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

class Mesh
{
public:
	std::vector <Vertex> m_vertices;
	std::vector <GLuint> m_indices;
	std::vector <Texture> m_textures;
	VAO VAO;
	unsigned int m_instanceCount = 0;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, unsigned int instances, std::vector<glm::mat4> instancematrix);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f)
	);

	float calculateMinY();

};