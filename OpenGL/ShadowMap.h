#pragma once

#include "Common.h"

class ShadowMap
{
public:
	ShadowMap() : m_FBO{ 0 }, m_shadowMap{ 0 }, m_shadowWidth{ 0 }, m_shadowHeight{ 0 }{}
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	unsigned int GetShadowWidth() { return m_shadowWidth; }
	unsigned int GetShadowHeight() { return m_shadowHeight; }

protected:
	GLuint m_FBO, m_shadowMap;
	GLuint m_shadowWidth, m_shadowHeight;
};

