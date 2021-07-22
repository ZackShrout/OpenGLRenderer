#pragma once

#include "Common.h"

class Shader
{
public:
	Shader();
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
	void UseShader();
	void ClearShader();

public:
	GLuint m_shaderID, m_uniformProjection, m_uniformModel, m_uniformView, m_uniformAmbientIntensity, m_uniformAmbientColor, m_uniformDiffuseIntensity, m_uniformDirection;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

