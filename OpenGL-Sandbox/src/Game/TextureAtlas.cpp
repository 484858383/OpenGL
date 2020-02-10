#include "TextureAtlas.h"

#include<GLCore/Core/Log.h>

TextureAtlas::TextureAtlas()
{
	m_unitSize = 1.f / static_cast<float>(m_texturesPerRow);
}

std::vector<GLfloat> TextureAtlas::getUVCoordinates(const glm::ivec2& textureAtlasPos)
{
	return get().getUVCoordinatesImpl(textureAtlasPos);
}

std::vector<GLfloat> TextureAtlas::getUVCoordinates(int xAtlasPos, int yAtlasPos)
{
	return get().getUVCoordinatesImpl(xAtlasPos, yAtlasPos);
}

std::vector<GLfloat> TextureAtlas::getUVCoordinatesImpl(const glm::ivec2& textureAtlasPos)
{
	float x_min, x_max, y_min, y_max;
	x_min = textureAtlasPos.x * m_unitSize;
	x_max = x_min + m_unitSize;

	y_min = textureAtlasPos.y * m_unitSize;
	y_max = y_min + m_unitSize;

	if(x_min >= 1.f || y_min >= 1.f)
		LOG_WARN("texture coords: ({}, {}) exceed texture bounds", textureAtlasPos.x, textureAtlasPos.y);

	return
	{
	 x_max, y_min,
	 x_max, y_max,
	 x_min, y_max,
	 x_min, y_min,
	};
}

std::vector<GLfloat> TextureAtlas::getUVCoordinatesImpl(int xAtlasPos, int yAtlasPos)
{
	return getUVCoordinatesImpl(glm::ivec2(xAtlasPos, yAtlasPos));
}
