#include "Material.h"

void Material::UseMaterial(unsigned int specIntensityLocation, unsigned int shininessLocation)
{
	glUniform1f(specIntensityLocation, m_specularIntensity);
	glUniform1f(shininessLocation, m_shininess);
}
