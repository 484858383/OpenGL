#pragma once

#include<array>
#include<glm/glm.hpp>

#include"../WorldConstants.h"
#include"../Block/Block.h"
#include"../Block/ChunkBlock.h"

#include"../../OpenGL/Buffer/VertexArray.h"

class ChunkMeshBuilder;

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
private:
	int index(const glm::ivec3& pos);
	int index(int x, int y, int z);

	bool outOfBounds(const glm::ivec3& pos);
	bool outOfBounds(int x, int y, int z);
private:
	std::array<ChunkBlock, WorldConstants::ChunkVolume> m_blocks;
	glm::ivec2 m_position;

	VertexArray m_vertexArray;
};