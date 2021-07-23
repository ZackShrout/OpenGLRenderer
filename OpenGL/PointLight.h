#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() : m_position{ glm::vec3(0.0f, 0.0f, 0.0f) }, m_constant{ 1.0f }, m_linear{ 0.0f }, m_exponent{ 0.0f }, Light() {}
	PointLight(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat r, GLfloat g, GLfloat b,
				GLfloat aIntensity, GLfloat dIntensity) 
		: m_position{ glm::vec3(xPos, yPos, zPos) }, m_constant{ constant }, m_linear{ linear }, m_exponent{ exponent }, Light(r, g, b, aIntensity, dIntensity) {}
	~PointLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int positionLocation,
		unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);

private:
	glm::vec3 m_position;
	GLfloat m_constant, m_linear, m_exponent;
};

