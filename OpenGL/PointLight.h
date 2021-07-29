#pragma once

#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light
{
public:
	PointLight() : Light() {}
	PointLight(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat near, GLfloat far, GLfloat r, GLfloat g, GLfloat b,
		GLfloat aIntensity, GLfloat dIntensity, GLfloat shadowWidth, GLfloat shadowHeight);
	~PointLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int positionLocation,
		unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);
	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane() { return m_farPlane; }
	glm::vec3 GetPosition() { return m_position; }

protected:
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	GLfloat m_constant{ 1.0f };
	GLfloat m_linear{ 0.0f };
	GLfloat m_exponent{ 0.0f };
	GLfloat m_farPlane{ 0.0 };
};

