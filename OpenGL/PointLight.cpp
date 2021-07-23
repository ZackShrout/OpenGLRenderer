#include "PointLight.h"

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
