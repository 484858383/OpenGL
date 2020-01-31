#pragma once

#include<glad/glad.h>
#include<string>

class Texture
{
public:
	Texture(const std::string& filename);

	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& rhs) = delete;

	Texture(Texture&& other);
	Texture& operator=(Texture&& rhs);

	~Texture();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNumberChannels() const { return m_numberChannels; }

	void bind() const { glBindTexture(GL_TEXTURE_2D, m_textureID); }
	void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
private:
	void deleteData();
private:
	GLuint m_textureID;
	int m_width, m_height, m_numberChannels;
};