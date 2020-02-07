#include "Texture.h"

#include<stb_image/stb_image.h>
#include<GLCore/Core/Log.h>

Texture::Texture(const std::string& filename)
{
	stbi_set_flip_vertically_on_load(1);
	std::string fullFilepath = "res/textures/" + filename;

	unsigned char* data = stbi_load(fullFilepath.c_str(), &m_width, &m_height, &m_numberChannels, 0);

	if(!data)
	{
		LOG_WARN("failed to load texture file: {}", fullFilepath);
		LOG_WARN("loading error texture instead");
		data = stbi_load("res/textures/error.png", &m_width, &m_height, &m_numberChannels, 0);
		if(!data)
			LOG_CRITICAL("failed to load error texture");
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::Texture(Texture&& other)
	:m_textureID(other.m_textureID), m_width(other.m_width),
	 m_height(other.m_height), m_numberChannels(other.m_numberChannels)
{
	other.m_textureID = 0;
	other.m_width = 0;
	other.m_height = 0;
	other.m_numberChannels = 0;
}

Texture& Texture::operator=(Texture&& rhs)
{
	if(this == &rhs)
		return *this;

	deleteData();

	m_textureID = rhs.m_textureID;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_numberChannels = rhs.m_numberChannels;

	rhs.m_textureID = 0;
	rhs.m_width = 0;
	rhs.m_height = 0;
	rhs.m_numberChannels = 0;

	return *this;
}

Texture::~Texture()
{
	deleteData();
}

void Texture::deleteData()
{
	glDeleteTextures(1, &m_textureID);
	m_width = 0;
	m_height = 0;
	m_numberChannels = 0;
	m_textureID = 0;
}
