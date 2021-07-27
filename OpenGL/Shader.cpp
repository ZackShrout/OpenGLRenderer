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

void Shader::SetSpotLight(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
	assert(lightCount <= MAX_SPOT_LIGHTS);

	glUniform1i(m_uniformSpotLightCount, lightCount);

	for (unsigned int i{ 0 }; i < lightCount; i++)
	{
		sLight[i].UseLight(m_uniformSpotLight[i].uniformAmbientIntensity, m_uniformSpotLight[i].uniformColor, m_uniformSpotLight[i].uniformDiffuseIntensity,
			m_uniformSpotLight[i].uniformPosition, m_uniformSpotLight[i].uniformDirection, m_uniformSpotLight[i].uniformConstant, m_uniformSpotLight[i].uniformLinear, 
			m_uniformPointLight[i].uniformExponent, m_uniformSpotLight[i].uniformEdge);
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(m_uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(m_uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(m_uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::UseShader()
{
	assert(m_shaderID > 0);
	glUseProgram(m_shaderID);
}

void Shader::ClearShader()
{
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
	m_uniformSpotLightCount = glGetUniformLocation(m_shaderID, "spotLightCount");

	for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].base.color", i);
		m_uniformPointLight[i].uniformColor = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].base.ambientIntensity", i);
		m_uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].base.diffuseIntensity", i);
		m_uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].position", i);
		m_uniformPointLight[i].uniformPosition = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].constant", i);
		m_uniformPointLight[i].uniformConstant = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].linear", i);
		m_uniformPointLight[i].uniformLinear = glGetUniformLocation(m_shaderID, locationBuffer);
		
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLight[%d].exponent", i);
		m_uniformPointLight[i].uniformExponent = glGetUniformLocation(m_shaderID, locationBuffer);
	}

	for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.base.color", i);
		m_uniformSpotLight[i].uniformColor = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.base.ambientIntensity", i);
		m_uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.base.diffuseIntensity", i);
		m_uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.position", i);
		m_uniformSpotLight[i].uniformPosition = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.constant", i);
		m_uniformSpotLight[i].uniformConstant = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.linear", i);
		m_uniformSpotLight[i].uniformLinear = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].point.exponent", i);
		m_uniformSpotLight[i].uniformExponent = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].direction", i);
		m_uniformSpotLight[i].uniformDirection = glGetUniformLocation(m_shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLight[%d].edge", i);
		m_uniformSpotLight[i].uniformEdge = glGetUniformLocation(m_shaderID, locationBuffer);
	}

	m_uniformTexture = glGetUniformLocation(m_shaderID, "tex");
	m_uniformDirectionalLightTransform = glGetUniformLocation(m_shaderID, "directionalLightSpaceTransform");
	m_uniformDirectionalShadowMap = glGetUniformLocation(m_shaderID, "dirShadowMap");
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
