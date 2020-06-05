#pragma once

#include"../../OpenGL/Mesh.h"

struct ChunkMesh
{
	Mesh blockMesh;
	unsigned blockIndexCount = 0;

	Mesh waterMesh;
	unsigned waterIndexCount = 0;

	void clear()
	{
		blockMesh.clear();
		waterMesh.clear();

		blockIndexCount = 0;
		waterIndexCount = 0;
	}
};