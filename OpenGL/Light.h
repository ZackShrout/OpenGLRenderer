#pragma once

#include "Common.h"
#include "ShadowMap.h"

class Light
{
public:
	Light() {}
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity, GLfloat shadowWidth, GLfloat shadowHeight);
	~Light() {}

	ShadowMap* GetShadowMap() { return m_shadowMap; }

protected:
	glm::vec3 m_color{1.0f, 1.0f, 1.0f};
	GLfloat m_ambientIntensity{ 1.0f };
	GLfloat m_diffuseIntensity{ 0.0f };

	glm::mat4 m_lightProjection{};
	
	ShadowMap* m_shadowMap{};
};

