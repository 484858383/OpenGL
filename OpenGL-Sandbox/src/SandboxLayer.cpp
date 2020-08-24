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
	handleCollision();

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
					m_world.setBlock(std::floor(lastPosition.x), std::floor(lastPosition.y), std::floor(lastPosition.z), ChunkBlock::grass);
					glm::vec3 pos = {std::floor(lastPosition.x), std::floor(lastPosition.y), std::floor(lastPosition.z)};
					m_boundingBoxes.emplace_back(pos, glm::vec3(1, 1, 1));
				}
				break;
			}
			ray.step(m_rayStep);
			lastPosition = position;
		}
	}
}

void SandboxLayer::handleCollision()
{
	//displacements are used to change the position of the cameras bounding box so that it doesnt clip into blocks
	glm::vec3 displacements = {0.2f, 2.f, 0.2f};
	AABB cameraBox = {m_camera.position - displacements, m_camera.size};

	for(auto& box : m_boundingBoxes)
	{
		float dx = 0;
		float dy = 0;
		float dz = 0;

		//distances used to calculate deltas.
		glm::vec3 minMinDistance = cameraBox.minExtent - box.minExtent + cameraBox.size;
		glm::vec3 minMaxDistance = cameraBox.minExtent - box.maxExtent + cameraBox.size - 0.5f * box.size;

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
				dy -= 1.5f;
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
				LOG_INFO("jump");
				m_camera.position.y -= dy;
				m_camera.velocity.y = 0;
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
	//for(int y = -1; y <= 1; y++)
	//for(int x = -1; x <= 1; x++)
	//for(int z = -1; z <= 1; z++)
	//{
	//	glm::vec3 position = glm::vec3(x, y, z) - m_camera.position;

	//	if(x == 0 || y == 0 || z == 0)
	//		return;
	//	if(m_world.getBlock(position.x, position.y, position.z).getData().hasCollision)
	//	{
	//		
	//		m_boundingBoxes.emplace_back(position, glm::vec3(1, 1, 1));
	//	}
	//}

	auto block = m_world.getBlock(std::floor(m_camera.position.x), std::floor(m_camera.position.y) - 2, std::floor(m_camera.position.z));
	auto blockPos = m_camera.position;
	blockPos.y -= 2;
	if(block.getData().hasCollision)
	{
		//LOG_INFO("{}", block.getID());
		glm::vec3 pos = {std::floor(m_camera.position.x), std::floor(m_camera.position.y) - 2, std::floor(m_camera.position.z)};
		m_boundingBoxes.emplace_back(pos, glm::vec3(1, 1, 1));
	}
}
