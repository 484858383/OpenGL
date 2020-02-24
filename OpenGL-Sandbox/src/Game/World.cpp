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
		m_chunkBuilder.beginMesh(pair.second);
		m_chunkBuilder.buildMesh();
		m_chunkBuilder.endMesh();
	}
}

const Block& World::getBlock(const glm::ivec3& position) const
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !chunkExistsAt(chunkPos))
		return BlockDatabase::getBlock(ChunkBlock::air);
	return m_chunks.at(chunkPos).getBlock(position);
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
	m_chunks.at(chunkPos).setBlock(position, block);
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
	return y > WorldConstants::ChunkHeight || y < 0;
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
	position.x = x / 16;
	position.y = z / 16;
	return position;
}

void World::addChunk(int x, int z)
{
	m_chunks.emplace(glm::ivec2(x, z), Chunk(x, z));
}
