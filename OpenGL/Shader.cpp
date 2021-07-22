#include "Shader.h"

// PUBLIC
Shader::Shader() : m_shaderID{ 0 }, m_uniformModel{ 0 }, m_uniformProjection{ 0 }, m_uniformView{ 0 }, 
					m_uniformAmbientIntensity{ 0 }, m_uniformAmbientColor{ 0 }, m_uniformDiffuseIntensity{ 0 }, m_uniformDirection{ 0 }
{
	
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexFP, const char* fragmentFP)
{
	CompileShader(ReadFile(vertexFP).c_str(), ReadFile(fragmentFP).c_str());
}

std::string Shader::ReadFile(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to read " << filePath << "." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	
	fileStream.close();
	return content;
}

void Shader::UseShader()
{
	assert(m_shaderID > 0);
	glUseProgram(m_shaderID);
}

void Shader::ClearShader()
{
	assert(m_shaderID > 0);
	if (m_shaderID != 0)
	{
		glDeleteShader(m_shaderID);
		m_shaderID = 0;
	}

	m_uniformModel = 0;
	m_uniformProjection = 0;
	m_uniformView = 0;
}

// PRIVATE
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	m_shaderID = glCreateProgram();

	if (!m_shaderID)
	{
		std::cout << "Error creating shader." << std::endl;
		return; // ideally, this failure should be handle in the proram
	}

	AddShader(m_shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(m_shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	int result = 0;
	char log[1024]{ 0 };

	glLinkProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(log), nullptr, log);
		std::cout << "Error linking program: " << log << std::endl;
	}

	glValidateProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(log), nullptr, log);
		std::cout << "Error validating program: " << log << std::endl;
	}

	m_uniformModel = glGetUniformLocation(m_shaderID, "model");
	m_uniformProjection = glGetUniformLocation(m_shaderID, "projection");
	m_uniformView = glGetUniformLocation(m_shaderID, "view");
	m_uniformAmbientColor = glGetUniformLocation(m_shaderID, "directionalLight.color");
	m_uniformAmbientIntensity = glGetUniformLocation(m_shaderID, "directionalLight.ambientIntensity");
	m_uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, "directionalLight.diffuseIntensity");
	m_uniformDirection = glGetUniformLocation(m_shaderID, "directionalLight.direction");
	
}

void Shader::AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType)
{
	unsigned int theShader = glCreateShader(shaderType);
	const char* theCode[1];
	theCode[0] = shaderCode;
	int codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	int result = 0;
	char log[1024]{ 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(log), nullptr, log);
		std::cout << "Error compiling  " << shaderType << " shader: " << log << std::endl;
	}

	glAttachShader(theProgram, theShader);

	return;
}
