#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, unsigned int instances, std::vector<glm::mat4> instancematrix)
{
	Mesh::m_vertices = vertices;
	Mesh::m_indices = indices;
	Mesh::m_textures = textures;
	m_instanceCount = instances;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO instancedVBO(instancematrix);
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	//if there are more than on instance, link the instanced matrix
	if (m_instanceCount != 1)
	{
		instancedVBO.Bind();
		VAO.LinkAttrib(instancedVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
		VAO.LinkAttrib(instancedVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instancedVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		VAO.LinkAttrib(instancedVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	instancedVBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale,
	glm::vec3 location
)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		std::string num;
		std::string type = m_textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		m_textures[i].texUnit(shader, (type + num).c_str(), i);
		m_textures[i].Bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	// put the model in its correct location
	glUniform3f(glGetUniformLocation(shader.ID, "location"), location.x, location.y, location.z);

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	if (m_instanceCount != 1)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_instanceCount);
	}
}

float Mesh::calculateMinY()
{
	float minY = m_vertices[0].Position.y;
	for (int i = 1; i < m_vertices.size(); i++)
	{
		if (m_vertices[i].Position.y < minY)
		{
			minY = m_vertices[i].Position.y;
		}
	}
	return minY;
}