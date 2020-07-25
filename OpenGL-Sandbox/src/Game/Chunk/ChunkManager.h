#pragma once

#include<unordered_map>
#include<unordered_set>

#include"../Chunk/Chunk.h"
#include"../Chunk/ChunkMeshBuilder.h"
#include"../../Utility/Hasher.h"

using ChunkMap = std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>>;

class World;

class ChunkManager
{
public:
	ChunkManager(World& world);

	ChunkManager(const ChunkManager& rhs) = delete;
	ChunkManager& operator=(const ChunkManager& other) = delete;

	ChunkManager(ChunkManager&& rhs) = default;
	ChunkManager& operator=(ChunkManager&& other) = default;

	~ChunkManager() = default;

	Chunk& at(const glm::ivec2& key) { return *m_chunks.at(key); }
	const Chunk& at(const glm::ivec2& key) const { return *m_chunks.at(key); }

	Chunk& operator[](glm::ivec2& key) { return *m_chunks[key]; }
	Chunk& operator[](const glm::ivec2& key) { return *m_chunks[key]; }

	ChunkMap::iterator begin() { return m_chunks.begin(); }
	ChunkMap::iterator end() { return m_chunks.end(); }

	ChunkMap& getChunks() { return m_chunks; }

	void add(const glm::ivec2& key, std::unique_ptr<Chunk>&& chunk);

	void prepareChunkToBuild(const glm::ivec2& position);
	void prepareChunkToBuild(int x, int z) { prepareChunkToBuild({x, z}); }
	void prepareChunkToDelete(const glm::ivec2& position);
	void prepareChunkToDelete(int x, int z) { prepareChunkToDelete({x, z}); }

	void deleteChunks();
	void buildChunks();

	bool chunkExistsAt(const glm::ivec2& position) const;
private:
	World* m_world = nullptr;
	ChunkMeshBuilder m_chunkBuilder;
	ChunkMap m_chunks;
	std::unordered_set<glm::ivec2> m_keysToBuild;
	std::unordered_set<glm::ivec2> m_keysToDelete;
};