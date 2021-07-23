#pragma once

#include "Common.h"

class Light
{
public:
	Light() : m_color{ glm::vec3(1.0f, 1.0f, 1.0f) }, m_ambientIntensity{ 1.0f }, m_diffuseIntensity{ 0.0f } {}
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity) 
		: m_color{ glm::vec3(r, g, b) }, m_ambientIntensity{ aIntensity }, m_diffuseIntensity{ dIntensity } {}
	~Light() {}

protected:
	glm::vec3 m_color;
	GLfloat m_ambientIntensity, m_diffuseIntensity;
};

