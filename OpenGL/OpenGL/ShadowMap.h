#pragma once
#include "model.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


class ShadowMap
{
	public:
	ShadowMap(glm::vec3 lightPos);
	void sendLightSpaceMatrix(Shader shader);
	void bind(Shader shader);
	void drawShadows(Model model, Camera camera);
	Shader getShader() { return m_shader; }

	private:
		void initBuffers();
		void initShaders();
		void initMatrices();

		unsigned int m_FBO;
		unsigned int m_shadowMap;
		unsigned int m_shadowWidth = 2048, m_shadowHeight = 2048;
		glm::mat4 m_orthgonalProjection;
		glm::mat4 m_lightView;
		glm::vec3 m_lightPos;
		glm::mat4 m_lightSpaceMatrix;
		Shader 	m_shader = Shader("shadow.vert", "shadow.frag");
		
};

