#pragma once

#include "Common.h"
#include "Mesh.h"
#include "Shader.h"

class Texture
{
public:
	Texture() : m_textureID{ 0 }, m_width{ 0 }, m_height{ 0 }, m_bitDepth{ 0 }, m_filePath{ nullptr } {}
	Texture(const char* filePath) : m_textureID{ 0 }, m_width{ 0 }, m_height{ 0 }, m_bitDepth{ 0 }, m_filePath{ filePath } {}
	~Texture() { ClearTexture(); }

	bool LoadTexture();
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();

private:
	GLuint m_textureID;
	int m_width, m_height, m_bitDepth;
	const char* m_filePath;
};

class Skybox
{
public:
	Skybox() {};
	Skybox(std::vector<std::string> faceLocations);
	~Skybox() {};

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	Mesh* m_skyMesh{};
	Shader* m_skyShader{};

	GLuint m_textureID{ 0 };
	GLuint m_uniformProjection{ 0 }, m_uniformView{ 0 };
};