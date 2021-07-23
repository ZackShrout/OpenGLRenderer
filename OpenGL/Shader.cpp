#include "Shader.h"

// PUBLIC
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

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(m_uniformDirectionalLight.uniformAmbientIntensity, m_uniformDirectionalLight.uniformColor, m_uniformDirectionalLight.uniformDiffuseIntensity,
		m_uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLight(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
	assert(lightCount <= MAX_POINT_LIGHTS);

	glUniform1i(m_uniformPointLightCount, lightCount);

	for (unsigned int i{ 0 }; i < lightCount; i++)
	{
		pLight[i].UseLight(m_uniformPointLight[i].uniformAmbientIntensity, m_uniformPointLight[i].uniformColor, m_uniformPointLight[i].uniformDiffuseIntensity,
			m_uniformPointLight[i].uniformPosition, m_uniformPointLight[i].uniformConstant, m_uniformPointLight[i].uniformLinear, m_uniformPointLight[i].uniformExponent);
	}
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
	m_uniformDirectionalLight.uniformColor = glGetUniformLocation(m_shaderID, "directionalLight.base.color");
	m_uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(m_shaderID, "directionalLight.base.ambientIntensity");
	m_uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, "directionalLight.base.diffuseIntensity");
	m_uniformDirectionalLight.uniformDirection = glGetUniformLocation(m_shaderID, "directionalLight.direction");
	m_uniformEyePosition = glGetUniformLocation(m_shaderID, "eyePosition");
	m_uniformSpecularIntensity = glGetUniformLocation(m_shaderID, "material.specularIntensity");
	m_uniformShininess = glGetUniformLocation(m_shaderID, "material.shininess");
	m_uniformPointLightCount = glGetUniformLocation(m_shaderID, "pointLightCount");

	for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.color", i);
		m_uniformPointLight[i].uniformColor = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.ambientIntensity", i);
		m_uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.diffuseIntensity", i);
		m_uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].position", i);
		m_uniformPointLight[i].uniformPosition = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].constant", i);
		m_uniformPointLight[i].uniformConstant = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].linear", i);
		m_uniformPointLight[i].uniformLinear = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].exponent", i);
		m_uniformPointLight[i].uniformExponent = glGetUniformLocation(m_shaderID, locationBuffer);
	}
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
