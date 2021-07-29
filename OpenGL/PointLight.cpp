#include "PointLight.h"

PointLight::PointLight(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat near, GLfloat far, 
	GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity, GLfloat shadowWidth, GLfloat shadowHeight) : 
	m_position{ glm::vec3(xPos, yPos, zPos) }, m_constant{ constant }, m_linear{ linear }, m_exponent{ exponent }, m_farPlane{ far },
	Light(r, g, b, aIntensity, dIntensity, shadowWidth, shadowHeight)
{
	float aspect = shadowWidth / shadowHeight;
	m_lightProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);

	m_shadowMap = new OmniShadowMap();
	m_shadowMap->Init((int)shadowWidth, (int)shadowHeight);
}

void PointLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation,
							unsigned int positionLocation, unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation)
{
	glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
	glUniform3f(positionLocation, m_position.x, m_position.y, m_position.z);
	glUniform1f(constantLocation, m_constant);
	glUniform1f(linearLocation, m_linear);
	glUniform1f(exponentLocation, m_exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	// POSITIVE_X and NEGATIVE_X
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	// POSITIVE_Y and NEGATIVE_Y
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	// POSOTOVE_Z and NEGATIVE_Z
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	lightMatrices.emplace_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	return lightMatrices;
}
