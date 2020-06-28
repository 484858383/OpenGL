#include "Plains.h"

#include"../WorldConstants.h"
#include"../../Chunk/Chunk.h"

PlainsBiome::PlainsBiome(const NoiseData& nd)
	:Biome(nd)
{
}

void PlainsBiome::placeSurfaceBlock(Chunk& chunk, const glm::ivec3& position)
{
	if(position.y >= WorldConstants::WaterLevel)
	{
		chunk.setBlock(position, ChunkBlock::grass);
	}
	else
	{
		chunk.setBlock(position, ChunkBlock::sand);
	}
}

void PlainsBiome::placeSubSurfaceBlock(Chunk& chunk, const glm::ivec3& position) 
{
	if(position.y >= WorldConstants::WaterLevel)
	{
		chunk.setBlock(position, ChunkBlock::dirt);
	}
	else
	{
		chunk.setBlock(position, ChunkBlock::sand);
	}
}

void PlainsBiome::placeUndergroundBlock(Chunk& chunk, const glm::ivec3& position)
{
	if(position.y == 0)
	{
		chunk.setBlock(position, ChunkBlock::bottom);
		return;
	}

	chunk.setBlock(position, ChunkBlock::stone);
}

void PlainsBiome::placeFillBlock(Chunk& chunk, const glm::ivec3& position)
{
	if(position.y >= WorldConstants::WaterLevel)
	{
		chunk.setBlock(position, ChunkBlock::air);
	}
	else
	{
		chunk.setBlock(position, ChunkBlock::water);
	}
}

void PlainsBiome::placeTree(Chunk& chunk, const glm::ivec3& position)
{
}

void PlainsBiome::placeFoliage(Chunk& chunk, const glm::ivec3& position)
{
}
