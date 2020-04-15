#pragma once

#include<array>
#include<glm/vec2.hpp>
#include"WorldConstants.h"

class Chunk;

class TerrainGenerator
{
public:
	TerrainGenerator() = default;
	~TerrainGenerator() = default;

	ChunkHeightMap generateHeightMap(const glm::ivec2& position);
	void generateBlockData(Chunk& chunk, ChunkHeightMap& hm);
private:
	int index(int x, int y);
	float TerrainGenerator::bilerp(float x1, float x2, float z1, float z2, float x, float z, ChunkHeightMap& hm);
	void setHeight(int x, int z, int h, ChunkHeightMap& hm) { hm.at(index(x, z)) = h; }
	int heightAt(int x, int z, ChunkHeightMap& hm) { return hm.at(index(x, z)); }
};