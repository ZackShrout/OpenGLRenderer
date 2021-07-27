#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light() {}
	DirectionalLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat shadowWidth, GLfloat shadowHeight);
	~DirectionalLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation);
	glm::mat4 CalculateLightTransform();

private:
	glm::vec3 m_direction{ 0.0f, -1.0f, 0.0f };
};

