#pragma once

#include<glm/glm.hpp>

class Block
{
	struct BlockData;
public:
	Block() = default;

	const BlockData& getData() const { return m_data; }
	int getID() const { return m_data.ID; }

protected:
	struct BlockData
	{
		int ID = -1;
		bool isTransparent;
		bool isFoliage;
		glm::ivec2 textureCoords;
	} m_data;
};

class AirBlock : public Block
{
public:
	AirBlock()
	{
		m_data.ID = 0;
		m_data.isFoliage = false;
		m_data.isTransparent = true;
		m_data.textureCoords = {0, 0};
	}
};

class GrassBlock : public Block
{
public:
	GrassBlock()
	{
		m_data.ID = 1;
		m_data.isFoliage = false;
		m_data.isTransparent = false;
		m_data.textureCoords = {1, 0};
	}
};