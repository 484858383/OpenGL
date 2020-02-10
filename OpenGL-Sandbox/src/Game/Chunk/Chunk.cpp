#include "Chunk.h"

#include"../Block/Database.h"

Chunk::Chunk()
{
	m_blocks.fill(ChunkBlock::air);
}

const Block& Chunk::getBlock(const glm::ivec3& pos)
{
	return getBlock(pos.x, pos.y, pos.z);
}

const Block& Chunk::getBlock(int x, int y, int z)
{
	if(outOfBounds(x, y, z))
		return BlockDatabase::getBlock(ChunkBlock::air);

	return BlockDatabase::getBlock(m_blocks[index(x, y, z)]);
}

void Chunk::setBlock(const glm::ivec3& pos, ChunkBlock block)
{
	setBlock(pos.x, pos.y, pos.z, block);
}

void Chunk::setBlock(int x, int y, int z, ChunkBlock block)
{
	if(outOfBounds(x, y, z))
		return;

	 m_blocks[index(x, y, z)] = block;
}

const glm::ivec2& Chunk::getPosition()
{
	return m_position;
}

void Chunk::setPosition(const glm::ivec2 pos)
{
	m_position = pos;
}

void Chunk::setPosition(int x, int z)
{
	m_position = glm::ivec2(x, z);
}

int Chunk::index(const glm::ivec3& pos)
{
	return index(pos.x, pos.y, pos.z);
}

int Chunk::index(int x, int y, int z)
{
	return y * (16 * 16) + z * 16 + x;
}

bool Chunk::outOfBounds(const glm::ivec3& pos)
{
	return outOfBounds(pos.x, pos.y, pos.z);
}

bool Chunk::outOfBounds(int x, int y, int z)
{
	return x < 0 || y < 0 || z < 0
		|| x > 15 || z > 15 || y > WorldConstants::ChunkHeight - 1;
}