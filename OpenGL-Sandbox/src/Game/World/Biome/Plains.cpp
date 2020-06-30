#include "Plains.h"

#include"../WorldConstants.h"
#include"../../Chunk/Chunk.h"
#include"../World.h"

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

void PlainsBiome::placeTree(World& world, const glm::ivec3& position)
{
	if(position.y <= WorldConstants::WaterLevel)
		return;

	for(int x = -2; x < 3; x++)
		for(int z = -2; z < 3; z++)
		{
			if(world.getBlock(position.x + x, position.y + WorldConstants::TreeHeight - 3, position.z + z) == ChunkBlock::air)
				world.setBlock(position.x + x, position.y + WorldConstants::TreeHeight - 3, position.z + z, ChunkBlock::oak_leaves);
		}

	for(int y = 0; y < 3; y++)
		for(int x = -2 + y; x < 3 - y; x++)
			for(int z = -2 + y; z < 3 - y; z++)
			{
				if(world.getBlock(position.x + x, position.y + y + WorldConstants::TreeHeight - 2, position.z + z) == ChunkBlock::air)
					world.setBlock(position.x + x, position.y + y + WorldConstants::TreeHeight - 2, position.z + z, ChunkBlock::oak_leaves);
			}

	for(int i = 1; i < WorldConstants::TreeHeight - 1; i++)
	{
		auto block = world.getBlock(position.x, position.y + i, position.z);
		if(block == ChunkBlock::air || block == ChunkBlock::oak_leaves)
			world.setBlock(position.x, position.y + i, position.z, ChunkBlock::oak_log);
	}
}

void PlainsBiome::placeFoliage(Chunk& chunk, const glm::ivec3& position)
{
}
