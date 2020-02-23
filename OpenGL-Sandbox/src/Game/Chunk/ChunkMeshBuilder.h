#pragma once

#include<vector>

class Chunk;
class ChunkMesh;
class Block;
enum class direction;

///this class currently only builds the solid block data, discluding water and foliage data, this will be implemented in the future
class ChunkMeshBuilder
{
public:
	ChunkMeshBuilder() = default;

	void beginMesh(Chunk& chunk);
	void buildMesh();
	void endMesh();
private:
	void addFaceToMesh(int x, int y, int z, const Block& block, direction dir);
private:
	Chunk* m_chunk = nullptr;
	ChunkMesh* m_chunkMesh = nullptr;
	unsigned m_indexCounter = 0;

	std::vector<float> m_blockMeshData;
	std::vector<float> m_textureData;
	std::vector<unsigned> m_indices;
};