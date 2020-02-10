#pragma once

#include"../../OpenGL/Buffer/VertexArray.h"

struct Mesh
{
	VertexArray blockMesh;
	VertexArray waterMesh;
	VertexArray foliageMesh;
};

class ChunkMesh
{
public:
	ChunkMesh() = default;
	ChunkMesh(const ChunkMesh&) = delete;
	ChunkMesh& operator=(const ChunkMesh&) = delete;

	ChunkMesh(ChunkMesh&&) = default;
	ChunkMesh& operator=(ChunkMesh&&) = default;

	const Mesh& getMesh() const { return m_mesh; }

	bool& hasBlockMesh() const { m_hasBlockMesh; }
	bool& hasWaterMesh() const { m_hasWaterMesh; }
	bool& hasFoliageMesh() const { m_hasFoliageMesh; }
private:
	Mesh m_mesh;

	bool m_hasBlockMesh = false;
	bool m_hasWaterMesh = false;
	bool m_hasFoliageMesh = false;
};