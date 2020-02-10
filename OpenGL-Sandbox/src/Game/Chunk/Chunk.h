#pragma once

#include<array>
#include<glm/glm.hpp>

#include"../WorldConstants.h"
#include"../Block/Block.h"
#include"../Block/ChunkBlock.h"
#include"ChunkMesh.h"

class Chunk
{
public:
	Chunk();

	Chunk(const Chunk&) = delete;
	Chunk& operator=(const Chunk&) = delete;

	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;

	const Block& getBlock(const glm::ivec3& pos);
	const Block& getBlock(int x, int y, int z);

	void setBlock(const glm::ivec3& pos, ChunkBlock block);
	void setBlock(int x, int y, int z, ChunkBlock block);

	const glm::ivec2& getPosition();
	void setPosition(const glm::ivec2 pos);
	void setPosition(int x, int z);

	const ChunkMesh& getMesh() const { return m_mesh; }
private:
	int index(const glm::ivec3& pos);
	int index(int x, int y, int z);

	bool outOfBounds(const glm::ivec3& pos);
	bool outOfBounds(int x, int y, int z);
private:
	std::array<ChunkBlock, WorldConstants::ChunkVolume> m_blocks;
	glm::ivec2 m_position;

	ChunkMesh m_mesh;
};