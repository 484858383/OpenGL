#include "TerrainGenerator.h"

#include"../Chunk/Chunk.h"
#include"../../Utility/PerlinNoise.h"

#include"Biome/Plains.h"
#include"Biome/Archipelago.h"

#include<chrono>

TerrainGenerator::TerrainGenerator()
{

	NoiseData mountains;
	mountains.ampltude = 48;
	mountains.octaves = 6;
	mountains.frequency = 1.4f;
	mountains.persistence = 2.3f;
	mountains.smooth = 100.f;
	mountains.seed = 0;// std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	NoiseData plains;
	plains.ampltude = 38;
	plains.octaves = 6;
	plains.frequency = 2.f;
	plains.persistence = 1.6f;
	plains.smooth = 200.f;
	plains.seed = 0;

	NoiseData archipelago;
	archipelago.ampltude = 28;
	archipelago.octaves = 6;
	archipelago.frequency = 1.5f;
	archipelago.persistence = 1.9f;
	archipelago.smooth = 150.f;
	archipelago.seed = 0;

	NoiseData biomeNoiseData;
	biomeNoiseData.ampltude = 28;
	biomeNoiseData.octaves = 6;
	biomeNoiseData.frequency = 1.5f;
	biomeNoiseData.persistence = 2.5f;
	biomeNoiseData.smooth = 1000.f;
	biomeNoiseData.seed = 0;

	m_biomeNoise.setNoiseFunction(biomeNoiseData);

	m_biomes.emplace_back(std::make_unique<PlainsBiome>(plains));
	m_biomes.emplace_back(std::make_unique<ArchipelagoBiome>(archipelago));
}

ChunkHeightMap TerrainGenerator::generateHeightMap(const glm::ivec2& position)
{
	ChunkHeightMap hm;
	m_biomeMap.fill(BiomeType::plains); //clear the biome map for new chunk

	BiomeType lastBiome;
	
	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	for(int z = 0; z < WorldConstants::ChunkSize; z++)
	{
		if((x % 3) == 0 && (z % 3) == 0)
		{
			int wx = x + WorldConstants::ChunkSize * position.x;
			int wz = z + WorldConstants::ChunkSize * position.y;

			auto biomeValue = m_biomeNoise.Noise2D(wx, wz);
			int biomeIndex = 0;

			if(biomeValue > 25.f)
			{
				biomeIndex = static_cast<int>(BiomeType::archipelago);
				setBiome(x, z, BiomeType::archipelago);
				lastBiome = BiomeType::archipelago;
			}
			else
			{
				biomeIndex = static_cast<int>(BiomeType::plains);
				setBiome(x, z, BiomeType::plains);
				lastBiome = BiomeType::plains;
			}

			auto h = m_biomes[biomeIndex]->getNoise2D(wx, wz);
			setHeight(x, z, h, hm);
		}
		else
		{
			setHeight(x, z, 16, hm);
			setBiome(x, z, lastBiome);
		}

	}

	//these are the coords of the blocks that were given a height previously and are going to be used for the interpolation calculations

	int x1 = -3;
	int x2 =  0;

	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	{
		int z1 = -3;
		int z2 =  0;

		if(x % 3 == 0 && x < 15)
		{
			x1 += 3;
			x2 += 3;
		}

		for(int z = 0; z < WorldConstants::ChunkSize; z++)
		{
			if(z % 3 == 0 && z < 15)
			{
				z1 += 3;
				z2 += 3;
			}

			if((x % 3) == 0 && (z % 3) == 0)
				continue; //these heights are already calculated

			auto h = bilerp(x1, x2, z1, z2, x, z, hm);

			setHeight(x, z, h, hm);
		}
	}

	return hm;
}

void TerrainGenerator::generateBlockData(Chunk& chunk, ChunkHeightMap& hm)
{
	for(int y = 0; y < WorldConstants::ChunkHeight; y++)
		for(int x = 0; x < WorldConstants::ChunkSize; x++)
			for(int z = 0; z < WorldConstants::ChunkSize; z++)
			{
				int height = heightAt(x, z, hm);

				BiomeType biome = biomeAt(x, z);

				if(y == height)
					m_biomes[static_cast<int>(biome)]->placeSurfaceBlock(chunk, {x, y, z});
				else if(y < height - 2)
					m_biomes[static_cast<int>(biome)]->placeUndergroundBlock(chunk, {x, y, z});
				else if(y < height)
					m_biomes[static_cast<int>(biome)]->placeSubSurfaceBlock(chunk, {x, y, z});
				else
					m_biomes[static_cast<int>(biome)]->placeFillBlock(chunk, {x, y, z});
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
