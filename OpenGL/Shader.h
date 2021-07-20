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
	unsigned int GetProjectionLocation();
	unsigned int GetModelLocation();
	unsigned int GetViewLocation();
	void UseShader();
	void ClearShader();

public:
	GLuint m_shaderID, m_uniformProjection, m_uniformModel, m_uniformView;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

