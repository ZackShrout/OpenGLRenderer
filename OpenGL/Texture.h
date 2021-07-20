#pragma once

#include "Common.h"
#include "Vendor/stb/stb_image.h"

class Texture
{
public:
	Texture() : m_textureID{ 0 }, m_width{ 0 }, m_height{ 0 }, m_bitDepth{ 0 }, m_filePath{ nullptr } {}
	Texture(char* filePath) : m_textureID{ 0 }, m_width{ 0 }, m_height{ 0 }, m_bitDepth{ 0 }, m_filePath{ filePath } {}
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint m_textureID;
	int m_width, m_height, m_bitDepth;
	char* m_filePath;
};

