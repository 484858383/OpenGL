#include "ChunkMeshBuilder.h"

#include"Chunk.h"
#include"ChunkMesh.h"
#include"../TextureAtlas.h"
#include"../World/World.h"

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
		0, 0, 1,
		0, 0, 0,
		1, 0, 0,
	};
	std::vector<float> xFace1 =
	{
		0, 1, 0,
		0, 0, 0,
		1, 0, 1,
		1, 1, 1,
	};
	std::vector<float> xFace2 =
	{
		0, 1, 1,
		0, 0, 1,
		1, 0, 0,
		1, 1, 0,
	};

	std::vector<float> faceNormals =
	{
		 0,  0,  1, //front
		 0,  0, -1, //back
		-1,  0,  0, //left
		 1,  0,  0, //right
		 0,  1,  0, //top
		 0, -1,  0  //bottom
	};

	//unsure about these
	std::vector<float> xFaceNormals =
	{
		1, 0, -1, //xface 1
		1, 0,  1  //xface 2
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
	xFace1, xFace2
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
		if(block == ChunkBlock::air)
			continue;

		glm::ivec3 worldPosition = position + glm::ivec3(WorldConstants::ChunkSize * m_chunk->getPosition().x, 0,													   			  WorldConstants::ChunkSize * m_chunk->getPosition().y);

		tryToAddFace(worldPosition, position, right, block, direction::right);
		tryToAddFace(worldPosition, position, left, block, direction::left);
		tryToAddFace(worldPosition, position, back, block, direction::back);
		tryToAddFace(worldPosition, position, front, block, direction::front);
		tryToAddFace(worldPosition, position, up, block, direction::top);
		tryToAddFace(worldPosition, position, down, block, direction::bottom);
	}
	
}

void ChunkMeshBuilder::endMesh()
{
	m_chunk->m_blockMesh.addAttributes(m_mesh.blockMesh);
	m_chunk->m_waterMesh.addAttributes(m_mesh.waterMesh);
	m_chunk->m_translucentBlocksMesh.addAttributes(m_mesh.translucentBlockMesh);

	m_mesh.clear();

	m_chunk = nullptr;
#if SHOW_TIME
	LOG_INFO("time taken to build chunk: {}ms", m_clock.getMilliseconds());
#endif
}

void ChunkMeshBuilder::tryToAddFace(const glm::ivec3& worldPosition, const glm::ivec3& localPosition,
									const glm::ivec3& displacement, const Block& block, direction dir)
{
	auto& displacedBlock = m_world->getBlock(worldPosition + displacement);

	if(displacedBlock.getData().isTransparent || displacedBlock.getData().isTranslucent)
	{
		if(displacedBlock == ChunkBlock::air || (!block.getData().isTransparent && !block.getData().isTranslucent))
		{
			if(block == ChunkBlock::water)
				addFaceToMesh(localPosition.x, localPosition.y, localPosition.z,
							  block, dir, m_mesh.waterMesh, m_mesh.waterIndexCount);
			else if(block.getData().isTranslucent)
				addFaceToMesh(localPosition.x, localPosition.y, localPosition.z,
							  block, dir, m_mesh.translucentBlockMesh, m_mesh.translucentIndexCount);
			else if(block == ChunkBlock::grass_foliage || block.getData().isTransparent || block.getData().isTranslucent)
			{
				addFaceToMesh(localPosition.x, localPosition.y, localPosition.z,
							  block, direction::xFace1, m_mesh.translucentBlockMesh, m_mesh.translucentIndexCount);
				addFaceToMesh(localPosition.x, localPosition.y, localPosition.z,
							  block, direction::xFace2, m_mesh.translucentBlockMesh, m_mesh.translucentIndexCount);
			}
			else
				addFaceToMesh(localPosition.x, localPosition.y, localPosition.z,
							  block, dir, m_mesh.blockMesh, m_mesh.blockIndexCount);
		}
	}
}

void ChunkMeshBuilder::addFaceToMesh(int x, int y, int z, const Block& block, direction dir, Mesh& mesh, unsigned& indexCounter)
{
	float* faceData = nullptr;
	float* normals = faceNormals.data();
	const glm::ivec2* texCoords = nullptr;
	float faceBrightness = 1.f;
	int normalsOffset = 0;

	switch(dir)
	{
		case direction::front:
		{
			faceData = frontFace.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 0.6f;
			normalsOffset = 0;
		}
		break;
		case direction::back:
		{
			faceData = backFace.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 0.6f;
			normalsOffset = 3;
		}
		break;
		case direction::left:
		{
			faceData = leftFace.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 0.6f;
			normalsOffset = 6;
		}
		break;
		case direction::right:
		{
			faceData = rightFace.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 0.6f;
			normalsOffset = 9;
		}
		break;
		case direction::top:
		{
			faceData = topFace.data();
			texCoords = &block.getData().topTextureCoords;
			faceBrightness = 1.f;
			normalsOffset = 12;
		}
		break;
		case direction::bottom:
		{
			faceData = bottomFace.data();
			texCoords = &block.getData().bottomTextureCoords;
			faceBrightness = 0.3f;
			normalsOffset = 15;
		}
		break;
		case direction::xFace1:
		{
			faceData = xFace1.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 1.f;
			normals = xFaceNormals.data();
			normalsOffset = 0;
		}
		break;
		case direction::xFace2:
		{
			faceData = xFace2.data();
			texCoords = &block.getData().sideTextureCoords;
			faceBrightness = 1.f;
			normals = xFaceNormals.data();
			normalsOffset = 3;
		}
		break;
		default:
		break;
	}

	auto texCoordData = TextureAtlas::getUVCoordinates(*texCoords);

	auto& vertexPositions = mesh.positions;
	auto& vertexTextureCoords = mesh.textureCoords;
	auto& vertexBrightness = mesh.faceBrightness;
	auto& vertexNormals = mesh.normals;

	if(block.getData().isLiquid)
		faceBrightness = 1.f;

	for(int i = 0; i < 4; i++)
		vertexBrightness.emplace_back(faceBrightness);

	for(int i = 0; i < 12; i += 3)
	{
		vertexPositions.emplace_back(faceData[i]     + x + m_chunk->getPosition().x * WorldConstants::ChunkSize);
		vertexPositions.emplace_back(faceData[i + 1] + y);
		vertexPositions.emplace_back(faceData[i + 2] + z + m_chunk->getPosition().y * WorldConstants::ChunkSize);
		vertexNormals.emplace_back(normals[normalsOffset]);
		vertexNormals.emplace_back(normals[normalsOffset + 1]);
		vertexNormals.emplace_back(normals[normalsOffset + 2]);
	}

	vertexTextureCoords.insert(vertexTextureCoords.end(), texCoordData.begin(), texCoordData.end());

	mesh.indices.insert(mesh.indices.end(),
	{
		indexCounter, indexCounter + 1, indexCounter + 2,
		indexCounter + 2, indexCounter + 3, indexCounter,
	});
	indexCounter += 4;
}
