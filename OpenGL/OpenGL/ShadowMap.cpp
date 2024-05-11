#include "ShadowMap.h"
#include <glm/gtc/type_ptr.hpp>

ShadowMap::ShadowMap(glm::vec3 lightPos)
{
	m_lightPos = lightPos;
	initBuffers();
	initMatrices();
	initShaders();

}

void ShadowMap::sendLightSpaceMatrix(Shader shader)
{
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(m_lightSpaceMatrix));
}

void ShadowMap::bind(Shader shader)
{
	shader.Activate();
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glUniform1i(glGetUniformLocation(shader.ID, "shadowMap"), 2);
}

void ShadowMap::drawShadows(Model model, Camera camera)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, m_shadowWidth, m_shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::initBuffers()
{
	glGenFramebuffers(1, &m_FBO);
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::initShaders()
{
	m_shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(m_lightSpaceMatrix));
}

void ShadowMap::initMatrices()
{
	m_orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	m_lightView = glm::lookAt(20.0f * m_lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightSpaceMatrix = m_orthgonalProjection * m_lightView;
}

