#include "Cubemap.h"

#include<stb_image/stb_image.h>
#include<GLCore/Core/Log.h>
#include<array>


namespace
{
	std::array<std::string, 6> faces = {"_right", "_left", "_top", "_bottom", "_front", "_back"};
}

Cubemap::Cubemap(const std::string& filename)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	int width, height, numberChannels;

	for(int i = 0; i < 6; i++)
	{
		std::string fullFilepath = "res/textures/skybox/" + filename + faces[i] + ".png";

		unsigned char* data = stbi_load(fullFilepath.c_str(), &width, &height, &numberChannels, 0);

		if(data != nullptr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			LOG_CRITICAL("failed to load skybox texture(s)");
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

Cubemap::Cubemap(Cubemap&& other)
	:m_id(other.m_id)
{
	other.m_id = 0;
}

Cubemap& Cubemap::operator=(Cubemap&& rhs)
{
	if(this == &rhs)
		return *this;

	glDeleteTextures(1, &m_id);

	m_id = rhs.m_id;

	rhs.m_id = 0;
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &m_id);
}
