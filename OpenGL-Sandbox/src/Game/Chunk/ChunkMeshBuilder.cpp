#include "ChunkMeshBuilder.h"

#include"Chunk.h"
#include"ChunkMesh.h"
#include"../TextureAtlas.h"

#include<GLCore/Core/Log.h>

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
}

enum class direction : int
{
	front, back,
	left, right,
	top, bottom,
};


void ChunkMeshBuilder::beginMesh(Chunk& chunk)
{
	m_chunk = &chunk;
	m_chunkMesh = &chunk.m_mesh;
}

void ChunkMeshBuilder::buildMesh()
{
	assert(m_chunk && m_chunkMesh, "could not build chunk mesh");

	for(int y = 0; y < WorldConstants::ChunkHeight; y++)
	for(int x = 0; x < WorldConstants::ChunkSize; x++)
	for(int z = 0; z < WorldConstants::ChunkSize; z++)
	{
		glm::ivec3 position = {x, y, z};
		auto block = m_chunk->getBlock(position);
		if(block.getData().isTransparent)
			continue;

		if(m_chunk->getBlock(x + 1, y, z).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::right);
		}

		if(m_chunk->getBlock(x - 1, y, z).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::left);
		}

		if(m_chunk->getBlock(x, y + 1, z).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::top);
		}

		if(m_chunk->getBlock(x, y - 1, z).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::bottom);
		}

		if(m_chunk->getBlock(x, y, z + 1).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::front);
		}

		if(m_chunk->getBlock(x, y, z - 1).getData().isTransparent)
		{
			addFaceToMesh(x, y, z, m_chunk->getBlock(x, y, z), direction::back);
		}
	}
}

void ChunkMeshBuilder::endMesh()
{
	m_chunkMesh->getMesh().blockMesh.addAttribute("a_position", m_blockMeshData, 3);
	m_chunkMesh->getMesh().blockMesh.addAttribute("a_texCoords", m_textureData, 2);
	m_chunkMesh->getMesh().blockMesh.addIndexBuffer(m_indices);

	m_blockMeshData.clear();
	m_blockMeshData.shrink_to_fit();
	m_textureData.clear();
	m_textureData.shrink_to_fit();
	m_indices.clear();
	m_indices.shrink_to_fit();

	m_indexCounter = 0;
	m_chunk = nullptr;
	m_chunkMesh = nullptr;
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

	for(int i = 0; i < 12; i += 3)
	{
		m_blockMeshData.emplace_back(faceData[i] + x + m_chunk->getPosition().x * WorldConstants::ChunkSize);
		m_blockMeshData.emplace_back(faceData[i + 1] + y);
		m_blockMeshData.emplace_back(faceData[i + 2] + z + m_chunk->getPosition().y * WorldConstants::ChunkSize);
	}

	m_textureData.insert(m_textureData.end(), texCoordData.begin(), texCoordData.end());
	m_indices.insert(m_indices.end(),
					 {
						 m_indexCounter, m_indexCounter + 1, m_indexCounter + 2,
						 m_indexCounter + 2, m_indexCounter + 3, m_indexCounter,
					 });
	m_indexCounter += 4;
}
