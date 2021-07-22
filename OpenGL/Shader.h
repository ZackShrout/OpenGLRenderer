#pragma once

#include "Common.h"

class Shader
{
public:
	Shader() : m_shaderID{ 0 }, m_uniformModel{ 0 }, m_uniformProjection{ 0 }, m_uniformView{ 0 }, m_uniformAmbientIntensity{ 0 }, m_uniformAmbientColor{ 0 }, 
		m_uniformDiffuseIntensity{ 0 }, m_uniformDirection{ 0 }, m_uniformEyePosition{ 0 }, m_uniformSpecularIntensity{ 0 }, m_uniformShininess{ 0 } {}
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexFP, const char* fragmentFP);
	std::string ReadFile(const char* filePath);
	unsigned int GetProjectionLocation() { return m_uniformProjection; }
	unsigned int GetModelLocation() { return m_uniformModel; }
	unsigned int GetViewLocation() { return m_uniformView; }
	unsigned int GetAmbientIntensityLocation() { return m_uniformAmbientIntensity; }
	unsigned int GetAmbientColorLocation() { return m_uniformAmbientColor; }
	unsigned int GetDiffuseIntensityLocation() { return m_uniformDiffuseIntensity; }
	unsigned int GetDirectionLocation() { return m_uniformDirection; }
	unsigned int GetEyePositionLocation() { return m_uniformEyePosition; }
	unsigned int GetSpecularIntensityLocation() { return m_uniformSpecularIntensity; }
	unsigned int GetShininessLocation() { return m_uniformShininess; }
	void UseShader();
	void ClearShader();

public:
	GLuint m_shaderID, m_uniformProjection, m_uniformModel, m_uniformView, m_uniformAmbientIntensity, m_uniformAmbientColor, m_uniformDiffuseIntensity, m_uniformDirection,
		m_uniformEyePosition, m_uniformSpecularIntensity, m_uniformShininess;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

