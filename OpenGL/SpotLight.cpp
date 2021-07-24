#include "SpotLight.h"

void SpotLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int positionLocation,
	unsigned int directionLocation, unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation, unsigned int edgeLocation)
{
	glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
	glUniform3f(positionLocation, m_position.x, m_position.y, m_position.z);
	glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
	glUniform1f(constantLocation, m_constant);
	glUniform1f(linearLocation, m_linear);
	glUniform1f(exponentLocation, m_exponent);
	glUniform1f(edgeLocation, m_procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	m_position = pos;
	m_direction = dir;
}
