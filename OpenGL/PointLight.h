#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() : Light() {}
	PointLight(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat r, GLfloat g, GLfloat b,
				GLfloat aIntensity, GLfloat dIntensity) 
		: m_position{ glm::vec3(xPos, yPos, zPos) }, m_constant{ constant }, m_linear{ linear }, m_exponent{ exponent }, Light(r, g, b, aIntensity, dIntensity, 1024, 1024) {}
	~PointLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int positionLocation,
		unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);

protected:
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	GLfloat m_constant{ 1.0f };
	GLfloat m_linear{ 0.0f };
	GLfloat m_exponent{ 0.0f };
};

