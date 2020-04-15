#include "World.h"

#include"../Block/Database.h"
#include"../Renderer/Renderer.h"


World::World()
{
	/*for(int x = 0; x < 2; x++)
	for(int z = 0; z < 2; z++)
	{
		addChunk(x, z);
	}

	for(auto& pair : m_chunks)
	{
		m_chunkBuilder.beginMesh(pair.second, *this);
		m_chunkBuilder.buildMesh();
		m_chunkBuilder.endMesh();
	}*/
}

const Block& World::getBlock(const glm::ivec3& position) const
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !chunkExistsAt(chunkPos))
		return BlockDatabase::getBlock(ChunkBlock::air);

	auto blockPos = getBlockPosition(position);
	return m_chunks.at(chunkPos).getBlock(blockPos);
}

void World::setBlock(const glm::ivec3& position, ChunkBlock block)
{
	auto chunkPos = getChunkPosition(position);
	if(outOfBounds(position.y) || !chunkExistsAt(chunkPos))
		return;

	auto blockPos = getBlockPosition(position);
	m_chunks.at(chunkPos).setBlock(blockPos, block);
	m_keysToBuild.insert(chunkPos);

	if(blockPos.x == 0)
		prepareChunkToBuild(chunkPos.x - 1, chunkPos.y);
	if(blockPos.x == WorldConstants::ChunkSize - 1)
		prepareChunkToBuild(chunkPos.x + 1, chunkPos.y);
	if(blockPos.z == 0)
		prepareChunkToBuild(chunkPos.x, chunkPos.y - 1);
	if(blockPos.z == WorldConstants::ChunkSize - 1)
		prepareChunkToBuild(chunkPos.x, chunkPos.y + 1);
}

void World::update(const glm::vec3& cameraPosition)
{
	std::vector<glm::ivec2> addedPositions;

	//add chunks in player's radius
	for(int x = -WorldConstants::renderDistance; x <= WorldConstants::renderDistance; x++)
	for(int z = -WorldConstants::renderDistance; z <= WorldConstants::renderDistance; z++)
	{
		glm::ivec2 chunkPosition = {x + std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize),
									z + std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize)};

		if(!chunkExistsAt(chunkPosition))
		{
			addChunk(chunkPosition.x, chunkPosition.y);
			addedPositions.push_back(chunkPosition);
		}
	}

	//remove chunks outside player's radius
	for(auto& pair : m_chunks)
	{
		glm::ivec2 max = {std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize + WorldConstants::renderDistance),
						  std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize + WorldConstants::renderDistance)};
		glm::ivec2 min = {std::floor(cameraPosition.x / (float)WorldConstants::ChunkSize - WorldConstants::renderDistance),
						  std::floor(cameraPosition.z / (float)WorldConstants::ChunkSize - WorldConstants::renderDistance)};

		auto& position = pair.second.getPosition();

		if(position.x > max.x || position.x < min.x || position.y > max.y || position.y < min.y)
			prepareChunkToDelete(position);
	};

	for(auto& chunkPos : addedPositions)
	{
		prepareChunkToBuild(chunkPos);
		prepareChunkToBuild(chunkPos.x - 1, chunkPos.y);
		prepareChunkToBuild(chunkPos.x + 1, chunkPos.y);
		prepareChunkToBuild(chunkPos.x, chunkPos.y + 1);
		prepareChunkToBuild(chunkPos.x, chunkPos.y - 1);
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

bool World::chunkExistsAt(const glm::ivec2& position) const
{
	return m_chunks.find(position) != m_chunks.end();
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
	auto heightMap = m_terrainGen.generateHeightMap(chunk.getPosition());

	chunk.setHeightMap(heightMap);
	m_terrainGen.generateBlockData(chunk, heightMap);

	m_chunks.emplace(glm::ivec2(x, z), std::move(chunk));
}

void World::prepareChunkToBuild(const glm::ivec2& chunkPosition)
{
	if(!chunkExistsAt(chunkPosition))
		return;
	m_keysToBuild.insert(chunkPosition);
}

void World::prepareChunkToDelete(const glm::ivec2& chunkPosition)
{
	if(!chunkExistsAt(chunkPosition))
		return;
	m_keysToDelete.insert(chunkPosition);
}

void World::deleteChunks()
{
	for(auto& key : m_keysToDelete)
		m_chunks.erase(key);

	m_keysToDelete.clear();
}

void World::buildChunks()
{
	for(auto key : m_keysToBuild)
	{
		if(chunkExistsAt(key))
		{
			m_chunkBuilder.beginMesh(m_chunks.at(key), *this);
			m_chunkBuilder.buildMesh();
			m_chunkBuilder.endMesh();
		}
	}
	m_keysToBuild.clear();
}
