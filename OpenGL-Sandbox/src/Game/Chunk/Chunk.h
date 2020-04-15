#pragma once

#include<array>
#include<glm/glm.hpp>

#include"../World/WorldConstants.h"
#include"../World/TerrainGenerator.h"
#include"../Block/Block.h"
#include"../Block/ChunkBlock.h"

#include"../../OpenGL/Buffer/VertexArray.h"

class ChunkMeshBuilder;
class World;

class Chunk
{
	friend class ChunkMeshBuilder;
public:
	Chunk(int x, int y);

	Chunk(const Chunk&) = delete;
	Chunk& operator=(const Chunk&) = delete;

	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;

	const Block& getBlock(const glm::ivec3& pos) const;
	const Block& getBlock(int x, int y, int z) const;

	void setBlock(const glm::ivec3& pos, ChunkBlock block);
	void setBlock(int x, int y, int z, ChunkBlock block);

	const glm::ivec2& getPosition() const;
	void setPosition(const glm::ivec2 pos);
	void setPosition(int x, int z);

	const VertexArray& getVertexArray() const { return m_vertexArray; }
	unsigned getNumberIndices() const { return m_vertexArray.getNumberIndicies(); }

	void setHeightMap(const ChunkHeightMap& heightMap) { m_heightMap = heightMap; }
	int getHeightAt(int x, int z) const;
private:
	int index(const glm::ivec3& pos) const;
	int index(int x, int y, int z) const;

	bool outOfBounds(const glm::ivec3& pos) const;
	bool outOfBounds(int x, int y, int z) const;
private:
	std::array<ChunkBlock, WorldConstants::ChunkVolume> m_blocks;
	ChunkHeightMap m_heightMap;
	glm::ivec2 m_position;

	VertexArray m_vertexArray;
};