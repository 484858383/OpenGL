#pragma once

#include<string>
#include<glad/glad.h>

class Cubemap
{
public:
	Cubemap(const std::string& filename);

	Cubemap(const Cubemap& other) = delete;
	Cubemap& operator=(const Cubemap& rhs) = delete;

	Cubemap(Cubemap&& other);
	Cubemap& operator=(Cubemap&& rhs);

	~Cubemap();

	void bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_id); }
	void unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

	GLuint getID() const { return m_id; }
private:
	GLuint m_id;
};