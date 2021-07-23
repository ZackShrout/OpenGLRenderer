#include "DirectionalLight.h"

void DirectionalLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation)
{
	glUniform3f(ambientColorLocation, m_color.x, m_color.y, m_color.z);
	glUniform1f(ambientIntensityLocation, m_ambientIntensity);
	glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z);
	glUniform1f(diffuseIntensityLocation, m_diffuseIntensity);
}
