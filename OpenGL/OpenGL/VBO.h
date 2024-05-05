#pragma once

#include<glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 TexUV;
};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);
	VBO(std::vector<glm::mat4>& m4s);


	void Bind();
	void Unbind();
	void Delete();

};

