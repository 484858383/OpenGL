#pragma once

#include<glm/glm.hpp>

#include"TerrainGenerator.h"
#include"../Chunk/ChunkManager.h"

class World
{
public:
	World();

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	World(World&&) = default;
	World& operator=(World&&) = default;

	const Block& getBlock(const glm::ivec3& position) const;
	const Block& getBlock(int x, int y, int z) const { return getBlock({x, y, z}); }

	void setBlock(const glm::ivec3& position, ChunkBlock block);
	void setBlock(int x, int y, int z, ChunkBlock block) { setBlock({x, y, z}, block); }

	void update(const glm::vec3& cameraPosition);

	void batchChunks();
	void deleteChunks() { m_chunks.deleteChunks(); }
	void buildChunks() { m_chunks.buildChunks(); }
private:
	///bounds checking
	bool outOfBounds(int y) const;
	bool outOfBounds(const glm::ivec3& pos) { return outOfBounds(pos.y); };

	///position calculation
	glm::ivec2 getChunkPosition(int x, int z) const;
	glm::ivec2 getChunkPosition(const glm::ivec3& pos) const { return getChunkPosition(pos.x, pos.z); }
	glm::ivec3 getBlockPosition(int x, int y, int z) const;
	glm::ivec3 getBlockPosition(const glm::ivec3& position) const { return getBlockPosition(position.x, position.y, position.z); }

	void addChunk(int x, int z);
private:
	ChunkManager m_chunks;
	TerrainGenerator m_terrainGen;
};