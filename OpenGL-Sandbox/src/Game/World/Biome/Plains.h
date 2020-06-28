#pragma once

#include"Biome.h"

class PlainsBiome : public Biome
{
public:
	PlainsBiome(const NoiseData& nd);
	void placeSurfaceBlock(Chunk& chunk, const glm::ivec3& position) override;
	void placeSubSurfaceBlock(Chunk& chunk, const glm::ivec3& position) override;
	void placeUndergroundBlock(Chunk& chunk, const glm::ivec3& position) override;
	void placeFillBlock(Chunk& chunk, const glm::ivec3& position) override;

	void placeTree(Chunk& chunk, const glm::ivec3& position) override;
	void placeFoliage(Chunk& chunk, const glm::ivec3& position) override;
private:
};