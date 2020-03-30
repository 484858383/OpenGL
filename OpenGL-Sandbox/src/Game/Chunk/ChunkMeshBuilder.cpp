#include "ChunkMeshBuilder.h"

#include"Chunk.h"
#include"ChunkMesh.h"
#include"../TextureAtlas.h"
#include"../World.h"

#include<GLCore/Core/Log.h>

#define SHOW_TIME 0

namespace
{
	std::vector<float> frontFace =
	{
		0, 1, 1,
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
	};
	std::vector<float> backFace =
	{
		1, 1, 0,
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
	};
	std::vector<float> leftFace =
	{
		0, 1, 0,
		0, 0, 0,
		0, 0, 1,
		0, 1, 1,
	};
	std::vector<float> rightFace =
	{
		1, 1, 1,
		1, 0, 1,
		1, 0, 0,
		1, 1, 0,
	};
	std::vector<float> topFace =
	{
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,
		0, 1, 1,
	};
	std::vector<float> bottomFace =
	{
		1, 0, 1,
		1, 0, 0,
		0, 0, 0,
		0, 0, 1,
	};

	glm::ivec3 front = { 0,  0,  1};
	glm::ivec3 back =  { 0,  0, -1};
	glm::ivec3 left =  {-1,  0,  0};
	glm::ivec3 right = { 1,  0,  0};
	glm::ivec3 up =    { 0,  1,  0};
	glm::ivec3 down =  { 0, -1,  0};
}

enum class direction : int
{
	front, back,
	left, right,
	top, bottom,
};


void ChunkMeshBuilder::beginMesh(Chunk& chunk, World& world)
{
	m_chunk = &chunk;
	m_world = &world;
#if SHOW_TIME
	m_clock.reset();
#endif
}

void ChunkMeshBuilder::buildMesh()
{
	assert(m_chunk && m_world, "could not build chunk mesh, world or chunk ptr is null");

	for(int y = 0; y < WorldConstants::ChunkHeight; y++)
	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	for(int z = 0; z < WorldConstants::ChunkSize; z++)
	{
		glm::ivec3 position = {x, y, z};
		auto& block = m_chunk->getBlock(position);
		if(block.getData().isTransparent)
			continue;

		glm::ivec3 worldPosition = position + glm::ivec3(WorldConstants::ChunkSize * m_chunk->getPosition().x, 0,																   WorldConstants::ChunkSize * m_chunk->getPosition().y);


		if(m_world->getBlock(worldPosition + right).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::right);
		}

		if(m_world->getBlock(worldPosition + left).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::left);
		}

		if(m_world->getBlock(worldPosition + up).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::top);
		}

		if(m_world->getBlock(worldPosition + down).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::bottom);
		}

		if(m_world->getBlock(worldPosition + front).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::front);
		}

		if(m_world->getBlock(worldPosition + back).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, block, direction::back);
		}
	}
	
}

void ChunkMeshBuilder::endMesh()
{
	m_chunk->m_vertexArray.addAttributes(m_mesh);

	m_mesh.clear();
	m_indexCounter = 0;
	m_chunk = nullptr;
#if SHOW_TIME
	LOG_INFO("time taken to build chunk: {}ms", m_clock.getMilliseconds());
#endif
}

void ChunkMeshBuilder::addFaceToMesh(int x, int y, int z, const Block& block, direction dir)
{
	float* faceData = nullptr;
	const glm::ivec2* texCoords = nullptr;

	switch(dir)
	{
		case direction::front:
		{
			faceData = frontFace.data();
			texCoords = &block.getData().sideTextureCoords;
		}
		break;
		case direction::back:
		{
			faceData = backFace.data();
			texCoords = &block.getData().sideTextureCoords;
		}
		break;
		case direction::left:
		{
			faceData = leftFace.data();
			texCoords = &block.getData().sideTextureCoords;
		}
		break;
		case direction::right:
		{
			faceData = rightFace.data();
			texCoords = &block.getData().sideTextureCoords;
		}
		break;
		case direction::top:
		{
			faceData = topFace.data();
			texCoords = &block.getData().topTextureCoords;
		}
		break;
		case direction::bottom:
		{
			faceData = bottomFace.data();
			texCoords = &block.getData().bottomTextureCoords;
		}
		break;
		default:
		break;
	}

	auto texCoordData = TextureAtlas::getUVCoordinates(*texCoords);

	auto& vertexPositions = m_mesh.positions;
	auto& vertexTextureCoords = m_mesh.textureCoords;

	for(int i = 0; i < 12; i += 3)
	{
		vertexPositions.emplace_back(faceData[i]     + x + m_chunk->getPosition().x * WorldConstants::ChunkSize);
		vertexPositions.emplace_back(faceData[i + 1] + y);
		vertexPositions.emplace_back(faceData[i + 2] + z + m_chunk->getPosition().y * WorldConstants::ChunkSize);
	}

	vertexTextureCoords.insert(vertexTextureCoords.end(), texCoordData.begin(), texCoordData.end());

	m_mesh.indices.insert(m_mesh.indices.end(),
	{
		m_indexCounter, m_indexCounter + 1, m_indexCounter + 2,
		m_indexCounter + 2, m_indexCounter + 3, m_indexCounter,
	});
	m_indexCounter += 4;
}
