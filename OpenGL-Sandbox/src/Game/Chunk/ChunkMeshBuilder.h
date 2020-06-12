#pragma once

#include<vector>
#include<glm/vec3.hpp>
#include"ChunkMesh.h"
#include"../../Clock.h"

class Chunk;
class Block;
class World;
enum class direction;

///this class currently only builds the solid block data, discluding water and foliage data, this will be implemented in the future
class ChunkMeshBuilder
{
public:
	ChunkMeshBuilder() = default;

	void beginMesh(Chunk& chunk, World& world);
	void buildMesh();
	void endMesh();
private:
	void tryToAddFace(const glm::ivec3& worldPosition, const glm::ivec3& localPosition,
					  const glm::ivec3& displacement, const Block& block, direction dir);
	void addFaceToMesh(int x, int y, int z, const Block& block, direction dir, Mesh& mesh, unsigned& indexCounter);
private:
	Chunk* m_chunk = nullptr;
	World* m_world = nullptr;

	ChunkMesh m_mesh;

	Clock m_clock;
};