#pragma once

#include"../../../Utility/PerlinNoise.h"
#include<glm/glm.hpp>

enum class BiomeType : int
{
	plains = 0,
	archipelago = 1,
};

class Chunk;
class World;

class Biome
{
public:
	Biome() = default;

	Biome(const NoiseData& nd)
	{
		m_noiseData = nd;
		m_noise.setNoiseFunction(nd);
	}

	virtual ~Biome() = default;

	//block visible on surface, eg grass
	virtual void placeSurfaceBlock(Chunk& chunk, const glm::ivec3& position) = 0;

	//blocks under 1 or 2 below the surface block eg dirt
	virtual void placeSubSurfaceBlock(Chunk& chunk, const glm::ivec3& position) = 0;

	//blocks below sub surface blocks eg stone
	virtual void placeUndergroundBlock(Chunk& chunk, const glm::ivec3& position) = 0;

	virtual void placeFillBlock(Chunk& chunk, const glm::ivec3& position) = 0;

	virtual void placeTree(World& world, const glm::ivec3& position) = 0;
	virtual void placeFoliage(Chunk& chunk, const glm::ivec3& position) = 0;

	virtual float getNoise2D(float x, float y) { return m_noise.Noise2D(x, y); }
	virtual unsigned getTreeRarity() { return m_treeRarity; }
protected:
	PerlinNoise m_noise;
	NoiseData m_noiseData;
	//used in Random::getIntInRange(0, m_treeRarity) == 0 to see how often trees should be randomly placed
	unsigned m_treeRarity = 1000; 
};