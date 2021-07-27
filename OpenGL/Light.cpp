#include "Light.h"

Light::Light(GLfloat r, GLfloat g, GLfloat b, GLfloat aIntensity, GLfloat dIntensity, GLfloat shadowWidth, GLfloat shadowHeight)
	: m_color{ glm::vec3(r, g, b) }, m_ambientIntensity{ aIntensity }, m_diffuseIntensity{ dIntensity }
{
	m_shadowMap = new ShadowMap();
	m_shadowMap->Init(shadowWidth, shadowHeight);
}