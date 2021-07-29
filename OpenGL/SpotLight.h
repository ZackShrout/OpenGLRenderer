#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
public:
	SpotLight() : PointLight() {}
	SpotLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat edge, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear,
		GLfloat exponent, GLfloat near, GLfloat far, GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity, GLfloat shadowWidth, GLfloat shadowHeight) :
		m_direction{ glm::normalize(glm::vec3(xDir, yDir, zDir)) }, m_edge{ edge }, m_procEdge{ cosf(glm::radians(m_edge)) },
		PointLight(xPos, yPos, zPos, constant, linear, exponent, near, far, r, g, b, aIntensity, dIntensity, shadowWidth, shadowHeight) {}
	~SpotLight() {}

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int positionLocation,
		unsigned int directionLocation, unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation, unsigned int edgeLocation);
	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	void Toggle() { isOn = !isOn; }

private:
	glm::vec3 m_direction{ glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) };
	GLfloat m_edge{ 0.0f };
	GLfloat m_procEdge{ cosf(glm::radians(m_edge)) };
	bool isOn{ false };
};

