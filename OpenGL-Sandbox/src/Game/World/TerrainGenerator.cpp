#include "TerrainGenerator.h"

#include"../Chunk/Chunk.h"
#include"../../Utility/PerlinNoise.h"

#include<chrono>

ChunkHeightMap TerrainGenerator::generateHeightMap(const glm::ivec2& position)
{
	ChunkHeightMap hm;

	NoiseData mountains;
	mountains.ampltude = 48;
	mountains.octaves = 6;
	mountains.frequency = 1.4f;
	mountains.persistence = 2.3f;
	mountains.smooth = 100.f;
	mountains.seed = 0;// std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	NoiseData islands;
	islands.ampltude = -32;
	islands.octaves = 6;
	islands.frequency = 1.7f;
	islands.persistence = 2.4f;
	islands.smooth = 150.f;

	NoiseData desert;
	desert.ampltude = 33;
	desert.octaves = 6;
	desert.frequency = 1.7f;
	desert.persistence = 1.7f;
	desert.smooth = 150.f;
	desert.seed = 0;

	PerlinNoise pn(desert);

	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	for(int z = 0; z < WorldConstants::ChunkSize; z++)
	{
		if((x % 5) == 0 && (z % 5) == 0)
		{
			int wx = x + WorldConstants::ChunkSize * position.x;
			int wz = z + WorldConstants::ChunkSize * position.y;
			auto h = pn.Noise2D(wx, wz);
			setHeight(x, z, h, hm);
		}
		else
			setHeight(x, z, 16, hm);
	}

	//these are the coords of the blocks that were given a height previously and are going to be used for the interpolation calculations
	int x1 = 0;
	int x2 = 5;

	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	{
		int z1 = 0;
		int z2 = 5;

		if(x == 5 || x == 10)
		{
			x1 += 5;
			x2 += 5;
		}

		for(int z = 0; z < WorldConstants::ChunkSize; z++)
		{
			if(z == 5 || z == 10)
			{
				z1 += 5;
				z2 += 5;
			}

			if((x % 5) == 0 && (z % 5) == 0)
				continue; //these heights are already calculated

			auto h = bilerp(x1, x2, z1, z2, x, z, hm);

			setHeight(x, z, h, hm);
		}
	}

	return hm;
}

void TerrainGenerator::generateBlockData(Chunk& chunk, ChunkHeightMap& hm)
{
	//for(int y = 0; y < WorldConstants::ChunkHeight; y++)
	//	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	//		for(int z = 0; z < WorldConstants::ChunkSize; z++)
	//		{
	//			int height = heightAt(x, z, hm);
	//			if(y == height)
	//				chunk.setBlock(x, y, z, ChunkBlock::grass);
	//			else if(y < height - 2)
	//				chunk.setBlock(x, y, z, ChunkBlock::stone);
	//			else if(y < height)
	//				chunk.setBlock(x, y, z, ChunkBlock::dirt);
	//			else
	//				chunk.setBlock(x, y, z, ChunkBlock::air);

	//			if(y == 0)
	//				chunk.setBlock(x, y, z, ChunkBlock::bottom);
	//		}
	for(int y = 0; y < WorldConstants::ChunkHeight; y++)
		for(int x = 0; x < WorldConstants::ChunkSize; x++)
			for(int z = 0; z < WorldConstants::ChunkSize; z++)
			{
				int height = heightAt(x, z, hm);

				if(y < WorldConstants::WaterLevel)
				{
					if(y == height)
						chunk.setBlock(x, y, z, ChunkBlock::sand);
					else if(y < height - 2)
						chunk.setBlock(x, y, z, ChunkBlock::stone);
					else if(y < height)
						chunk.setBlock(x, y, z, ChunkBlock::sand);
					else
						chunk.setBlock(x, y, z, ChunkBlock::water);
				}

				else
				{
					if(y == height)
						chunk.setBlock(x, y, z, ChunkBlock::grass);
					else if(y < height - 2)
						chunk.setBlock(x, y, z, ChunkBlock::stone);
					else if(y < height)
						chunk.setBlock(x, y, z, ChunkBlock::dirt);
					else
						chunk.setBlock(x, y, z, ChunkBlock::air);
				}

				if(y == 0)
					chunk.setBlock(x, y, z, ChunkBlock::bottom);
			}
}

int TerrainGenerator::index(int x, int y)
{
	return y * WorldConstants::ChunkSize + x;
}

float TerrainGenerator::bilerp(float x1, float x2, float z1, float z2, float x, float z, ChunkHeightMap& hm)
{
	float q1, q2, q3, q4;
	q1 = hm[index(x1, z1)];
	q2 = hm[index(x1, z2)];
	q3 = hm[index(x2, z1)];
	q4 = hm[index(x2, z2)];
	float r1 = ((x2 - x) / (x2 - x1)) * q1 + ((x - x1) / (x2 - x1)) * q3;
	float r2 = ((x2 - x) / (x2 - x1)) * q2 + ((x - x1) / (x2 - x1)) * q4;
	float p = ((z2 - z) / (z2 - z1)) * r1 + ((z - z1) / (z2 - z1)) * r2;

	return p;
}
