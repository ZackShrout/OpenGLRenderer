#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity,
	GLfloat shadowWidth, GLfloat shadowHeight)
	: m_direction{ glm::vec3(xDir, yDir, zDir) }, Light(r, g, b, aIntensity, dIntensity, shadowWidth, shadowHeight)
{
	m_lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation)
{
	glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return m_lightProjection * glm::lookAt(-m_direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
