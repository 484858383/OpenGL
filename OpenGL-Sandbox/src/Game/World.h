#pragma once

#include<unordered_map>
#include<unordered_set>
#include<glm/glm.hpp>

#include"Chunk/Chunk.h"
#include"Chunk/ChunkMeshBuilder.h"
#include"../Utility/Hasher.h"

class World
{
public:
	World();

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	World(World&&) = default;
	World& operator=(World&&) = default;

	const Block& getBlock(const glm::ivec3& position) const;
	const Block& getBlock(int x, int y, int z) const;

	void setBlock(const glm::ivec3& position, ChunkBlock block);
	void setBlock(int x, int y, int z, ChunkBlock block);

	void update(const glm::vec3& cameraPosition);
	void batchChunks();
private:
	///bounds checking
	bool outOfBounds(const glm::ivec3& pos) const;
	bool outOfBounds(int y) const;
	bool chunkExistsAt(const glm::ivec2& position) const;

	///position calculation
	glm::ivec2 getChunkPosition(const glm::ivec3& pos) const;
	glm::ivec2 getChunkPosition(int x, int z) const;
	glm::ivec3 getBlockPosition(const glm::ivec3& position) const;
	glm::ivec3 getBlockPosition(int x, int y, int z) const;

	///chunk manipulation (map insert/build/delete)
	void addChunk(int x, int z);
	void prepareChunkToBuild(const glm::ivec2& chunkPosition);
	void prepareChunkToBuild(int x, int z) { prepareChunkToBuild({x, z}); }
	void prepareChunkToDelete(const glm::ivec2& chunkPosition);
	void deleteChunks();
	void buildChunks();
private:
	ChunkMeshBuilder m_chunkBuilder;
	std::unordered_map<glm::ivec2, Chunk> m_chunks;
	std::unordered_set<Chunk*> m_chunksToBuild;
	std::unordered_set<glm::ivec2> m_chunkKeysToDelete;
};