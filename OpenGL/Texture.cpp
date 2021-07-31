#include "Texture.h"
#include "Vendor/stb/stb_image.h"


bool Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(m_filePath, &m_width, &m_height, &m_bitDepth, 0);
	if (!texData)
	{
		std::cout << "Failed to find " << m_filePath << std::endl;
		return false;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

bool Texture::LoadTextureA()
{
	unsigned char* texData = stbi_load(m_filePath, &m_width, &m_height, &m_bitDepth, 0);
	if (!texData)
	{
		std::cout << "Failed to find " << m_filePath << std::endl;
		return false;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
	
	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &m_textureID);
	m_textureID = 0;
	m_width = 0;
	m_height = 0;
	m_bitDepth = 0;
	m_filePath = nullptr;
}

// **********************************************SKYBOX*********************************************************
Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// Shader setup
	m_skyShader = new Shader();
	m_skyShader->CreateFromFile("..\\OpenGL\\Shaders\\vskybox.glsl", "..\\OpenGL\\Shaders\\fskybox.glsl");

	m_uniformProjection = m_skyShader->GetProjectionLocation();
	m_uniformView = m_skyShader->GetViewLocation();

	// Texture Setup
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	int width{ 0 }, height{ 0 }, bitDepth{ 0 };

	for (int i{ 0 }; i < 6; i++)
	{
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData)
		{
			std::cout << "Failed to find " << faceLocations[i] << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh Setup
	unsigned int skyboxIndices[] = 
	{
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	// NOTE: UVs and normals ae not needed for the skybox, so they are set to zero
	float skyboxVertices[] = 
	{
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	m_skyMesh = new Mesh();
	m_skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	// remove transforms from the viewMatrix
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	
	glDepthMask(GL_FALSE);

	m_skyShader->UseShader();

	glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(m_uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	m_skyShader->Validate();

	m_skyMesh->RenderMesh();

	glDepthMask(GL_TRUE);
}
