#pragma once

#include<vector>
#include"ChunkMesh.h"

class Chunk;
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

	Mesh m_mesh;
	unsigned m_indexCounter = 0;
};