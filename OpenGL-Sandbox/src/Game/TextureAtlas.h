#pragma once

#include<vector>
#include<glm/glm.hpp>
#include<glad/glad.h>

#include"../OpenGL/Texture.h"

class TextureAtlas
{
public:
	TextureAtlas(const TextureAtlas&) = delete;
	TextureAtlas& operator=(const TextureAtlas&) = delete;

	TextureAtlas(TextureAtlas&&) = delete;
	TextureAtlas& operator=(TextureAtlas&&) = delete;

	~TextureAtlas() = default;

	static TextureAtlas& get() { static TextureAtlas ta; return ta; }

	static std::vector<GLfloat> getUVCoordinates(const glm::ivec2& textureAtlasPos);
	static std::vector<GLfloat> getUVCoordinates(int xAtlasPos, int yAtlasPos);
	static void bind();
private:
	TextureAtlas();
	std::vector<GLfloat> getUVCoordinatesImpl(const glm::ivec2& textureAtlasPos);
	std::vector<GLfloat> getUVCoordinatesImpl(int xAtlasPos, int yAtlasPos);
	void bindImpl() const;

	Texture m_texture;
	int m_texturesPerRow = 16;
	float m_unitSize;
};