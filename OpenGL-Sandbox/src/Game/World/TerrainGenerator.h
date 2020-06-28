#pragma once

#include<vector>
#include<glm/vec2.hpp>
#include<memory>

#include"WorldConstants.h"
#include"Biome/Biome.h"

class Chunk;

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator() = default;

	ChunkHeightMap generateHeightMap(const glm::ivec2& position);
	void generateBlockData(Chunk& chunk, ChunkHeightMap& hm);
private:
	int index(int x, int y);
	float TerrainGenerator::bilerp(float x1, float x2, float z1, float z2, float x, float z, ChunkHeightMap& hm);
	void setHeight(int x, int z, int h, ChunkHeightMap& hm) { hm.at(index(x, z)) = h; }
	void setBiome(int x, int z, BiomeType biome) { m_biomeMap.at(index(x, z)) = biome; }
	int heightAt(int x, int z, ChunkHeightMap& hm) { return hm.at(index(x, z)); }
	BiomeType biomeAt(int x, int z) { return m_biomeMap.at(index(x, z)); }

	std::array<BiomeType, WorldConstants::ChunkSize * WorldConstants::ChunkSize> m_biomeMap;
	std::vector<std::unique_ptr<Biome>> m_biomes;
	PerlinNoise m_biomeNoise;
};