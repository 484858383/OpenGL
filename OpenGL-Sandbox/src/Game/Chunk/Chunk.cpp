#include "Chunk.h"

#include"../Block/Database.h"

Chunk::Chunk(int x, int y)
	:m_position(x, y)
{
	m_blocks.fill(ChunkBlock::air);
}

const Block& Chunk::getBlock(const glm::ivec3& pos) const
{
	return getBlock(pos.x, pos.y, pos.z);
}

const Block& Chunk::getBlock(int x, int y, int z) const
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

	glm::ivec3 position = glm::ivec3(x, y, z);
	position.x += m_position.x * WorldConstants::ChunkSize;
	position.z += m_position.y * WorldConstants::ChunkSize;

	if(block == ChunkBlock::light_block)
		m_lightPositions.insert(position);
	if(block == ChunkBlock::air && m_lightPositions.find(position) != m_lightPositions.end())
		m_lightPositions.erase(position);

	 m_blocks[index(x, y, z)] = block;
}

const glm::ivec2& Chunk::getPosition() const
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

int Chunk::getHeightAt(int x, int z) const
{
	return m_heightMap.at(z * WorldConstants::ChunkSize + x);
}

int Chunk::index(const glm::ivec3& pos) const
{
	return index(pos.x, pos.y, pos.z);
}

int Chunk::index(int x, int y, int z) const
{
	return y * (WorldConstants::ChunkSize * WorldConstants::ChunkSize) + z * WorldConstants::ChunkSize + x;
}

bool Chunk::outOfBounds(const glm::ivec3& pos) const
{
	return outOfBounds(pos.x, pos.y, pos.z);
}

bool Chunk::outOfBounds(int x, int y, int z) const
{
	int max = WorldConstants::ChunkSize - 1;
	return x < 0 || y < 0 || z < 0
		|| x > max || z > max || y > WorldConstants::ChunkHeight - 1;
}
