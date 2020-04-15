#pragma once

#include<unordered_map>
#include<unordered_set>
#include<glm/glm.hpp>

#include"../Chunk/Chunk.h"
#include"../Chunk/ChunkMeshBuilder.h"
#include"../../Utility/Hasher.h"
#include"TerrainGenerator.h"

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

	void deleteChunks();
	void buildChunks();
private:
	///bounds checking
	bool outOfBounds(int y) const;
	bool outOfBounds(const glm::ivec3& pos) { return outOfBounds(pos.y); };
	bool chunkExistsAt(const glm::ivec2& position) const;

	///position calculation
	glm::ivec2 getChunkPosition(int x, int z) const;
	glm::ivec2 getChunkPosition(const glm::ivec3& pos) const { return getChunkPosition(pos.x, pos.z); }
	glm::ivec3 getBlockPosition(int x, int y, int z) const;
	glm::ivec3 getBlockPosition(const glm::ivec3& position) const { return getBlockPosition(position.x, position.y, position.z); }

	///chunk manipulation (map insert/build/delete)
	void addChunk(int x, int z);
	void prepareChunkToBuild(const glm::ivec2& chunkPosition);
	void prepareChunkToBuild(int x, int z) { prepareChunkToBuild({x, z}); }
	void prepareChunkToDelete(const glm::ivec2& chunkPosition);
private:
	ChunkMeshBuilder m_chunkBuilder;
	TerrainGenerator m_terrainGen;
	std::unordered_map<glm::ivec2, Chunk> m_chunks;
	std::unordered_set<glm::ivec2> m_keysToBuild;
	std::unordered_set<glm::ivec2> m_keysToDelete;
};