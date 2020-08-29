#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<GLCore/Core/Input.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>

#include<glfw/include/GLFW/glfw3.h>

#include"Game/TextureAtlas.h"
#include"Game/Block/Database.h"
#include"Game/Renderer/Renderer.h"

#include"Game/Chunk/ChunkMeshBuilder.h"

#include"Game/Physics/Ray.h"
#include"Clock.h"

//TODO:
//materials (maybe lower on the list)
//shadows
//day/night cycle maybe
//change terrain generation so that trees generate properly

using namespace GLCore;

SandboxLayer::SandboxLayer()
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	Utils::EnableGLDebugging();

	BlockDatabase::get();
	TextureAtlas::get();
	Renderer::init(m_camera);
	m_camera.speed = .5f;
	m_camera.position.y = 32.f;
	m_currentBlock = ChunkBlock::light_block;

	//this should and will be moved onto a new layer (maybe "uiLayer" or "2dLayer") when more 2d visuals are needed
	std::vector<GLfloat> crosshairVertices =
	{
		-1.f,  1.f, 0.f,
		-1.f, -1.f, 0.f,
		 1.f, -1.f, 0.f,
		 1.f,  1.f, 0.f,
	};

	std::vector<GLfloat> crosshairTextureCoords =
	{
		 1.f,  0.f,
		 1.f,  1.f,
		 0.f,  1.f,
		 0.f,  0.f,
	};

	std::vector<unsigned> crosshairIndices =
	{
		0, 1, 2, 2, 3, 0
	};

	Mesh crosshairMesh = {crosshairVertices, crosshairTextureCoords, crosshairIndices};
	m_textures.emplace("crosshair", "crosshair.png");

	m_crosshair.constructMesh(crosshairMesh, m_textures.at("crosshair"));

	m_crosshair.scale = {0.02f, 0.02f};
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	if(Input::IsKeyPressed(GLFW_KEY_ESCAPE) && Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		Application::Get().close();
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	static Clock c;

	m_camera.input();
	m_camera.update(ts);

	updateBoundingBoxes();
	handleCollisions();

	Renderer::clear();

	raycast(c);
	m_world.update(m_camera.position);

	Renderer::add2DModelToQueue(m_crosshair);

	m_world.batchChunks();
	Renderer::update(ts);

	
	m_world.buildChunks();
	m_world.deleteChunks();
}

void SandboxLayer::OnImGuiRender()
{
}

void SandboxLayer::raycast(Clock& clock)
{
	auto lookAt = m_camera.lookAt();
	auto current = m_camera.position;

	if((Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)
		|| Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2)) && clock.getSeconds() > m_timeToBreak)
	{
		clock.reset();
		Ray ray(m_camera);
		glm::vec3 lastPosition = {0, 0, 0};

		while(ray.length() <= m_blockBreakRange)
		{
			glm::ivec3 position(std::floor(ray.getPosition().x), std::floor(ray.getPosition().y), std::floor(ray.getPosition().z));
			auto block = m_world.getBlock(position.x, position.y, position.z);

			if(block != ChunkBlock::air)
			{
				if(Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && block.getData().breakable)
					m_world.setBlock(position.x, position.y, position.z, ChunkBlock::air);
				else if(Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
				{
					glm::vec3 pos = {std::floor(lastPosition.x), std::floor(lastPosition.y), std::floor(lastPosition.z)};
					auto aabb = getCameraBoundingBox();
					glm::vec3 floorCameraPos1 = {std::floor(aabb.position.x), std::floor(aabb.position.y), std::floor(aabb.position.z)};
					glm::vec3 floorCameraPos2 = {std::floor(aabb.position.x), std::floor(aabb.position.y) + 1, std::floor(aabb.position.z)};

					if(pos == floorCameraPos1 || pos == floorCameraPos2)
						break;

					m_world.setBlock(std::floor(lastPosition.x), std::floor(lastPosition.y), std::floor(lastPosition.z), m_currentBlock);
					m_boundingBoxes.emplace_back(pos, glm::vec3(1, 1, 1));
				}
				break;
			}
			ray.step(m_rayStep);
			lastPosition = position;
		}
	}
}

void SandboxLayer::handleCollisions()
{
	AABB cameraBox = getCameraBoundingBox();

	for(auto& box : m_boundingBoxes)
	{
		float dx = 0;
		float dy = 0;
		float dz = 0;

		//distances used to calculate deltas.
		//calculate the distances between min and max extents
		glm::vec3 minMinDistance = cameraBox.minExtent - box.minExtent + cameraBox.size;
		glm::vec3 minMaxDistance = cameraBox.minExtent - box.maxExtent + cameraBox.size - 0.5f * box.size;

		//check if one of the camera's extents is inside a given bounding boxes' extents
		//depending on which of the camera's extents (min or max extents) fall between the boxes extents,
		//the distance (dx/y/z) is set accordingly 
		//finally after checking each dimension i calculate which delta is the shortest and push the camera in that direction by that amount
		//this solution is likely not the fastest or most clean but this does make intuitive sense (at least to me), this solution was figured out,
		//completely by myself and will likely be improved on my next project that uses AABB collision
		if(cameraBox.maxExtent.x > box.minExtent.x)
		{
			if(cameraBox.maxExtent.x < box.maxExtent.x)
			{
				dx = minMinDistance.x;
			}
			else if(cameraBox.minExtent.x < box.maxExtent.x)
			{
				dx = minMaxDistance.x;
			}
		}

		if(cameraBox.maxExtent.y > box.minExtent.y)
		{
			if(cameraBox.maxExtent.y < box.maxExtent.y)
			{
				dy = minMinDistance.y;
				dy += 0.2f;
			}
			else if(cameraBox.minExtent.y < box.maxExtent.y)
			{
				dy = minMaxDistance.y;
				dy -= 1.3f;
			}
		}

		if(cameraBox.maxExtent.z > box.minExtent.z)
		{
			if(cameraBox.maxExtent.z < box.maxExtent.z)
			{
				dz = minMinDistance.z;
			}
			else if(cameraBox.minExtent.z < box.maxExtent.z)
			{
				dz = minMaxDistance.z;
			}
		}

		if(dx != 0 && dy != 0 && dz != 0)
		{
			float smallestDistance = glm::min(glm::min(glm::abs(dx), glm::abs(dy)), glm::abs(dz));

			if(smallestDistance == glm::abs(dx))
			{
				m_camera.position.x -= dx;
				m_camera.velocity.x = 0;

			}
			if(smallestDistance == glm::abs(dy))
			{
				m_camera.position.y -= dy;
				m_camera.velocity.y = 0;
				//if the box i colided with is above me, do not reset my jump
				if(box.position.y < cameraBox.position.y)
					m_camera.canJump = true;
			}
			if(smallestDistance == glm::abs(dz))
			{
				m_camera.position.z -= dz;
				m_camera.velocity.z = 0;

			}
		}
	}
}

void SandboxLayer::updateBoundingBoxes()
{
	m_boundingBoxes.clear();
	
	for(int x = -2; x <= 2; x++)
	for(int z = -2; z <= 2; z++)
	for(int y = -2; y <= 2; y++)
	{
		glm::vec3 blockPos = {std::floor(m_camera.position.x) + x, std::floor(m_camera.position.y) + y, std::floor(m_camera.position.z) + z};

		auto block = m_world.getBlock(static_cast<glm::ivec3>(blockPos));

		if(block.getData().hasCollision)
		{
			glm::vec3 pos = {std::floor(m_camera.position.x) + x, std::floor(m_camera.position.y) + y, std::floor(m_camera.position.z) + z};
			m_boundingBoxes.emplace_back(blockPos, glm::vec3(1, 1, 1));
		}
	}
}

AABB SandboxLayer::getCameraBoundingBox()
{
	//displacements are used to change the position of the cameras bounding box so that it doesnt clip into blocks
	static glm::vec3 displacements = {0.2f, 1.8f, 0.2f};
	return {m_camera.position - displacements, m_camera.size};
}
