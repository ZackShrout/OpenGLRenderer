#pragma once

#include "Common.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"



class Shader
{
public:
	Shader() : m_pointLightCount{ 0 }, m_spotLightCount{ 0 }, m_shaderID{ 0 }, m_uniformProjection{ 0 }, m_uniformModel{ 0 }, m_uniformView{ 0 }, m_uniformPointLightCount{ 0 },
		m_uniformSpotLightCount{ 0 }, m_uniformEyePosition{ 0 }, m_uniformSpecularIntensity{ 0 }, m_uniformShininess{ 0 }, m_uniformDirectionalLight{ 0, 0, 0, 0 } {}
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
	void UseShader();
	void ClearShader();

private:
	unsigned int m_pointLightCount, m_spotLightCount;
	GLuint m_shaderID, m_uniformProjection, m_uniformModel, m_uniformView, m_uniformPointLightCount,
		m_uniformSpotLightCount, m_uniformEyePosition, m_uniformSpecularIntensity, m_uniformShininess;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} m_uniformDirectionalLight;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} m_uniformPointLight[MAX_POINT_LIGHTS];

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
		GLuint uniformDirection;
		GLuint uniformEdge;
	} m_uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType);
};

