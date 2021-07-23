#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() : m_direction{ glm::vec3(0.0f, -1.0f, 0.0f) }, Light() {}
	DirectionalLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity)
		: m_direction{ glm::vec3(xDir, yDir, zDir) }, Light(r, g, b, aIntensity, dIntensity) {}
	~DirectionalLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation);

private:
	glm::vec3 m_direction;
};

