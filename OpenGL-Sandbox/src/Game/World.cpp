#include "World.h"

#include"Block/Database.h"
#include"Renderer/Renderer.h"

World::World()
{
	for(int x = 0; x < 4; x++)
	for(int z = 0; z < 4; z++)
	{
		addChunk(x, z);
	}

	for(auto& pair : m_chunks)
	{
		m_chunkBuilder.beginMesh(pair.second, *this);
		m_chunkBuilder.buildMesh();
		m_chunkBuilder.endMesh();
	}
}

const Block& World::getBlock(const glm::ivec3& position) const
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !chunkExistsAt(chunkPos))
		return BlockDatabase::getBlock(ChunkBlock::air);

	auto blockPos = getBlockPosition(position);
	return m_chunks.at(chunkPos).getBlock(blockPos);
}

const Block& World::getBlock(int x, int y, int z) const
{
	return getBlock({x, y, z});
}

void World::setBlock(const glm::ivec3& position, ChunkBlock block)
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !chunkExistsAt(chunkPos))
		return;

	auto blockPos = getBlockPosition(position);
	m_chunks.at(chunkPos).setBlock(blockPos, block);
}

void World::setBlock(int x, int y, int z, ChunkBlock block)
{
	setBlock({x, y, z}, block);
}

void World::update()
{

}

void World::batchChunks()
{
	for(auto& pair : m_chunks)
	{
		Renderer::addChunkToQueue(pair.second);
	}
}

bool World::outOfBounds(const glm::ivec3& pos) const
{
	return outOfBounds(pos.y);
}

bool World::outOfBounds(int y) const
{
	return y > WorldConstants::ChunkHeight - 1 || y < 0;
}

bool World::chunkExistsAt(const glm::ivec2& position) const
{
	return m_chunks.find(position) != m_chunks.end();
}

glm::ivec2 World::getChunkPosition(const glm::ivec3& pos) const
{
	return getChunkPosition(pos.x, pos.z);
}

glm::ivec2 World::getChunkPosition(int x, int z) const
{
	glm::ivec2 position;
	position.x = std::floor(x / 16.f);
	position.y = std::floor(z / 16.f);
	return position;
}

glm::ivec3 World::getBlockPosition(const glm::ivec3& position) const
{
	return getBlockPosition(position.x, position.y, position.z);
}

glm::ivec3 World::getBlockPosition(int x, int y, int z) const
{
	return {x % 16, y, z % 16};
}

void World::addChunk(int x, int z)
{
	m_chunks.emplace(glm::ivec2(x, z), Chunk(x, z));
}
