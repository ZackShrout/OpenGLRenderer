#pragma once

#include "Common.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"



class Shader
{
public:
	Shader(){}
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexFP, const char* fragmentFP);
	std::string ReadFile(const char* filePath);
	unsigned int GetProjectionLocation() { return m_uniformProjection; }
	unsigned int GetModelLocation() { return m_uniformModel; }
	unsigned int GetViewLocation() { return m_uniformView; }
	unsigned int GetAmbientIntensityLocation() { return m_uniformDirectionalLight.uniformAmbientIntensity; }
	unsigned int GetAmbientColorLocation() { return m_uniformDirectionalLight.uniformColor; }
	unsigned int GetDiffuseIntensityLocation() { return m_uniformDirectionalLight.uniformDiffuseIntensity; }
	unsigned int GetDirectionLocation() { return m_uniformDirectionalLight.uniformDirection; }
	unsigned int GetEyePositionLocation() { return m_uniformEyePosition; }
	unsigned int GetSpecularIntensityLocation() { return m_uniformSpecularIntensity; }
	unsigned int GetShininessLocation() { return m_uniformShininess; }
	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLight(PointLight* pLight, unsigned int lightCount);
	void SetSpotLight(SpotLight* sLight, unsigned int lightCount);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void UseShader();
	void ClearShader();

private:
	unsigned int m_pointLightCount{ 0 };
	unsigned int m_spotLightCount{ 0 };
	GLuint m_shaderID{ 0 };
	GLuint m_uniformProjection{ 0 };
	GLuint m_uniformModel{ 0 };
	GLuint m_uniformView{ 0 };
	GLuint m_uniformPointLightCount{ 0 };
	GLuint m_uniformSpotLightCount{ 0 };
	GLuint m_uniformEyePosition{ 0 };
	GLuint m_uniformSpecularIntensity{ 0 };
	GLuint m_uniformShininess{ 0 };
	GLuint m_uniformTexture{ 0 };
	GLuint m_uniformDirectionalLightTransform{ 0 };
	GLuint m_uniformDirectionalShadowMap{ 0 };

	struct
	{
		GLuint uniformColor{ 0 };
		GLuint uniformAmbientIntensity{ 0 };
		GLuint uniformDiffuseIntensity{ 0 };
		GLuint uniformDirection{ 0 };
	} m_uniformDirectionalLight{};

	struct
	{
		GLuint uniformColor{ 0 };
		GLuint uniformAmbientIntensity{ 0 };
		GLuint uniformDiffuseIntensity{ 0 };
		GLuint uniformPosition{ 0 };
		GLuint uniformConstant{ 0 };
		GLuint uniformLinear{ 0 };
		GLuint uniformExponent{ 0 };
	} m_uniformPointLight[MAX_POINT_LIGHTS]{};

	struct
	{
		GLuint uniformColor{ 0 };
		GLuint uniformAmbientIntensity{ 0 };
		GLuint uniformDiffuseIntensity{ 0 };
		GLuint uniformPosition{ 0 };
		GLuint uniformConstant{ 0 };
		GLuint uniformLinear{ 0 };
		GLuint uniformExponent{ 0 };
		GLuint uniformDirection{ 0 };
		GLuint uniformEdge{ 0 };
	} m_uniformSpotLight[MAX_SPOT_LIGHTS]{};

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

