#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<GLCore/Core/Input.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<chrono>

#include<glfw/include/GLFW/glfw3.h>

#include"Game/TextureAtlas.h"
#include"Game/Block/Database.h"
#include"Game/Renderer/Renderer.h"

#include"Game/Chunk/ChunkMeshBuilder.h"

#include"Game/Physics/Ray.h"

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
#include"Clock.h"
void SandboxLayer::OnUpdate(Timestep ts)
{
	static Clock c;

	m_camera.input();
	m_camera.update(ts);

	Renderer::clear();

	m_world.batchChunks();

	auto lookAt = m_camera.lookAt();
	auto current = m_camera.position;

	if((Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) 
	||  Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2)) && c.getSeconds() > 0.3f)
	{
		c.reset();
		Ray ray(m_camera);
		glm::vec3 lastPosition;

		while(ray.length() <= 5.f)
		{
			glm::ivec3 position(ray.getPosition().x, ray.getPosition().y, ray.getPosition().z);

			if(m_world.getBlock(position.x, position.y, position.z) != ChunkBlock::air)
			{
				if(Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
					m_world.setBlock(position.x, position.y, position.z, ChunkBlock::air);
				else
					m_world.setBlock(lastPosition.x, lastPosition.y, lastPosition.z, ChunkBlock::grass);
					break;
			}
			ray.step(0.1f);
			lastPosition = position;
		}
	}

	m_world.update();
	Renderer::update();
}

void SandboxLayer::OnImGuiRender()
{
}
