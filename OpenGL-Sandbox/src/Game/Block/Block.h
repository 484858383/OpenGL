#pragma once

#include<glm/glm.hpp>

struct BlockData
{
	int ID = -1;
	bool isTransparent = true;
	bool isFoliage = false;
	bool hasCollision = false;
	bool isLiquid = false;
	bool breakable = false;
	glm::ivec2 topTextureCoords;
	glm::ivec2 bottomTextureCoords;
	glm::ivec2 sideTextureCoords;
};

class Block
{
public:
	Block() = default;

	const BlockData& getData() const { return m_data; }
	int getID() const { return m_data.ID; }
protected:
	BlockData m_data;
};

class AirBlock : public Block
{
public:
	AirBlock()
	{
		m_data.ID = 0;
		m_data.isFoliage = false;
		m_data.isTransparent = true;
		m_data.hasCollision = false;
		m_data.isLiquid = false;
		m_data.breakable = false;
		m_data.topTextureCoords = {0, 0};
		m_data.bottomTextureCoords = {0, 0};
		m_data.sideTextureCoords = {0, 0};
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
		m_data.hasCollision = true;
		m_data.isLiquid = false;
		m_data.breakable = true;
		m_data.topTextureCoords = {1, 0};
		m_data.sideTextureCoords = {2, 0};
		m_data.bottomTextureCoords = {3, 0};
	}
};

class DirtBlock : public Block
{
public:
	DirtBlock()
	{
		m_data.ID = 2;
		m_data.isFoliage = false;
		m_data.isTransparent = false;
		m_data.hasCollision = true;
		m_data.isLiquid = false;
		m_data.breakable = true;
		m_data.topTextureCoords = {3, 0};
		m_data.sideTextureCoords = {3, 0};
		m_data.bottomTextureCoords = {3, 0};
	}
};

class StoneBlock : public Block
{
public:
	StoneBlock()
	{
		m_data.ID = 3;
		m_data.isFoliage = false;
		m_data.isTransparent = false;
		m_data.hasCollision = true;
		m_data.isLiquid = false;
		m_data.breakable = true;
		m_data.topTextureCoords = {4, 0};
		m_data.sideTextureCoords = {4, 0};
		m_data.bottomTextureCoords = {4, 0};
	}
};

class BottomBlock : public Block
{
public:
	BottomBlock()
	{
		m_data.ID = 4;
		m_data.isFoliage = false;
		m_data.isTransparent = false;
		m_data.hasCollision = true;
		m_data.isLiquid = false;
		m_data.breakable = false;
		m_data.topTextureCoords = {0, 1};
		m_data.bottomTextureCoords = {0, 1};
		m_data.sideTextureCoords = {0, 1};
	}
};