#pragma once

#include "Common.h"

class Light
{
public:
	Light() : m_color{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_direction{ glm::vec3(0.0f, -1.0f, 0.0f) }, m_ambientIntensity{ 1.0f }, m_diffuseIntensity{ 0.0f } {}
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) 
		: m_color{ glm::vec3(r, g, b) }, m_direction{ glm::vec3(xDir, yDir, zDir) }, m_ambientIntensity{ aIntensity }, m_diffuseIntensity{ dIntensity } {}
	~Light() {}
	
	// TODO: change all to unsigned int
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

private:
	glm::vec3 m_color, m_direction;
	GLfloat m_ambientIntensity, m_diffuseIntensity;
};

