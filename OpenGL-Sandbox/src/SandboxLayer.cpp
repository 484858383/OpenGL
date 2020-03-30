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

void SandboxLayer::OnUpdate(Timestep ts)
{
	m_camera.input();
	m_camera.update(ts);

	Renderer::clear();

	m_world.batchChunks();

	auto lookAt = m_camera.lookAt();
	auto current = m_camera.position;

	if(GLCore::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
	{
		glm::vec3 position = m_camera.position;
		glm::vec3 lookAt = m_camera.lookAt();
		for(int i = 0; i < 50; i++)
		{
			position += 1.f * lookAt;
			if(m_world.getBlock(position.x, position.y, position.z) != ChunkBlock::air)
			{
				m_world.setBlock(position.x, position.y, position.z, ChunkBlock::air);
				break;
				///vertex array replacement
			}
		}
		//m_world.setBlock(0, y++, 0, ChunkBlock::air);
	}

	m_world.update();
	Renderer::update();
}

void SandboxLayer::OnImGuiRender()
{
}
