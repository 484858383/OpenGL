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
	m_camera.speed = 1.f;
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

	Renderer::clear();

	raycast(c);
	m_world.update(m_camera.position);

	Renderer::add2DModelToQueue(m_crosshair);

	m_world.batchChunks();
	Renderer::update(ts);

	
	m_world.buildChunks();
	m_world.deleteChunks();

	//LOG_INFO("fps: {}", 1 / ts);
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
		glm::vec3 lastPosition;

		while(ray.length() <= m_blockBreakRange)
		{
			glm::ivec3 position(std::floor(ray.getPosition().x), std::floor(ray.getPosition().y), std::floor(ray.getPosition().z));
			auto block = m_world.getBlock(position.x, position.y, position.z);

			if(block != ChunkBlock::air)
			{
				if(Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && block.getData().breakable)
					m_world.setBlock(position.x, position.y, position.z, ChunkBlock::air);
				else if(Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
					m_world.setBlock(lastPosition.x, lastPosition.y, lastPosition.z, ChunkBlock::grass_foliage);
				break;
			}
			ray.step(m_rayStep);
			lastPosition = position;
		}
	}
}
