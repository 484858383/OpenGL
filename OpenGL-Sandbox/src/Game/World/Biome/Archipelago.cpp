#include "Archipelago.h"

#include"../WorldConstants.h"
#include"../../Chunk/Chunk.h"

ArchipelagoBiome::ArchipelagoBiome(const NoiseData& nd)
	:Biome(nd)
{
}

void ArchipelagoBiome::placeSurfaceBlock(Chunk& chunk, const glm::ivec3& position)
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

void ArchipelagoBiome::placeSubSurfaceBlock(Chunk& chunk, const glm::ivec3& position)
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

void ArchipelagoBiome::placeUndergroundBlock(Chunk& chunk, const glm::ivec3& position)
{
	if(position.y == 0)
	{
		chunk.setBlock(position, ChunkBlock::bottom);
		return;
	}

	chunk.setBlock(position, ChunkBlock::stone);
}

void ArchipelagoBiome::placeFillBlock(Chunk & chunk, const glm::ivec3 & position)
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

void ArchipelagoBiome::placeTree(Chunk& chunk, const glm::ivec3& position)
{
}

void ArchipelagoBiome::placeFoliage(Chunk& chunk, const glm::ivec3& position)
{
}
