#include "World.h"

#include"../Block/Database.h"
#include"../Renderer/Renderer.h"

World::World()
	:m_chunks(*this), m_terrainGen(*this)
{
	/*for(int x = 0; x < 2; x++)
	for(int z = 0; z < 2; z++)
	{
		addChunk(x, z);
	}

	buildChunks();*/
}

const Block& World::getBlock(const glm::ivec3& position) const
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !m_chunks.chunkExistsAt(chunkPos))
		return BlockDatabase::getBlock(ChunkBlock::air);

	auto blockPos = getBlockPosition(position);
	return m_chunks.at(chunkPos).getBlock(blockPos);
}

void World::setBlock(const glm::ivec3& position, ChunkBlock block)
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !m_chunks.chunkExistsAt(chunkPos))
		return;

	auto blockPos = getBlockPosition(position);
	m_chunks.at(chunkPos).setBlock(blockPos, block);;
	m_chunks.prepareChunkToBuild(chunkPos);

	if(blockPos.x == 0)
		m_chunks.prepareChunkToBuild(chunkPos.x - 1, chunkPos.y);
	if(blockPos.x == WorldConstants::ChunkSize - 1)
		m_chunks.prepareChunkToBuild(chunkPos.x + 1, chunkPos.y);
	if(blockPos.z == 0)
		m_chunks.prepareChunkToBuild(chunkPos.x, chunkPos.y - 1);
	if(blockPos.z == WorldConstants::ChunkSize - 1)
		m_chunks.prepareChunkToBuild(chunkPos.x, chunkPos.y + 1);
}

void World::update(const glm::vec3& cameraPosition)
{
	std::vector<glm::ivec2> addedPositions;

	//add chunks in player's radius
	for(int x = -WorldConstants::RenderDistance; x <= WorldConstants::RenderDistance; x++)
	for(int z = -WorldConstants::RenderDistance; z <= WorldConstants::RenderDistance; z++)
	{
		glm::ivec2 chunkPosition = {x + std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize),
									z + std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize)};

		if(!m_chunks.chunkExistsAt(chunkPosition))
		{
			addChunk(chunkPosition.x, chunkPosition.y);
			addedPositions.push_back(chunkPosition);
		}
	}

	//remove chunks outside player's radius
	for(auto& pair : m_chunks)
	{
		glm::ivec2 max = {std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize + WorldConstants::RenderDistance),
						  std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize + WorldConstants::RenderDistance)};
		glm::ivec2 min = {std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize - WorldConstants::RenderDistance),
						  std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize - WorldConstants::RenderDistance)};

		auto& position = pair.second.getPosition();

		if(position.x > max.x || position.x < min.x || position.y > max.y || position.y < min.y)
			m_chunks.prepareChunkToDelete(position);
	};

	for(auto& chunkPos : addedPositions)
	{
		m_chunks.prepareChunkToBuild(chunkPos);
		m_chunks.prepareChunkToBuild(chunkPos.x - 1, chunkPos.y);
		m_chunks.prepareChunkToBuild(chunkPos.x + 1, chunkPos.y);
		m_chunks.prepareChunkToBuild(chunkPos.x, chunkPos.y + 1);
		m_chunks.prepareChunkToBuild(chunkPos.x, chunkPos.y - 1);
	}
}

void World::batchChunks()
{
	for(auto& pair : m_chunks)
	{
		Renderer::addChunkToQueue(pair.second);
	}
}

bool World::outOfBounds(int y) const
{
	return y > WorldConstants::ChunkHeight - 1 || y < 0;
}

glm::ivec2 World::getChunkPosition(int x, int z) const
{
	glm::ivec2 position;
	position.x = std::floor(x / static_cast<float>(WorldConstants::ChunkSize));
	position.y = std::floor(z / static_cast<float>(WorldConstants::ChunkSize));
	return position;
}

glm::ivec3 World::getBlockPosition(int x, int y, int z) const
{
	return {(WorldConstants::ChunkSize + (x % WorldConstants::ChunkSize)) % WorldConstants::ChunkSize,
			y,
			(WorldConstants::ChunkSize + (z % WorldConstants::ChunkSize)) % WorldConstants::ChunkSize};
}

void World::addChunk(int x, int z)
{
	Chunk chunk(x, z);
	m_terrainGen.generateChunkTerrain(chunk);
	m_chunks.add(glm::ivec2(x, z), std::move(chunk));

	m_terrainGen.generateTreeData();
}
