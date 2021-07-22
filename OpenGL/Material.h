#pragma once

#include "Common.h"

class Material
{
public:
	Material() : m_specularIntensity{ 0.0f }, m_shininess{ 0.0f } {}
	Material(GLfloat specularIntensity, GLfloat shininess) : m_specularIntensity{ specularIntensity }, m_shininess{ shininess } {}
	~Material() {}

	void UseMaterial(unsigned int specIntensityLocation, unsigned int shininessLocation);

private:
	GLfloat m_specularIntensity, m_shininess;
};

