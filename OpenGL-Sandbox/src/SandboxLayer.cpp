#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<GLCore/Core/Input.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>
#include<glm/glm.hpp>

#include<glfw/include/GLFW/glfw3.h>

#include"Game/TextureAtlas.h"
#include"Game/Block/Database.h"

#include"Game/Chunk/ChunkMeshBuilder.h"

using namespace GLCore;

SandboxLayer::SandboxLayer()
	:m_shader("TextureVert", "TextureFrag"), m_texture("atlas.png"), m_chunk(0, 0)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	Utils::EnableGLDebugging();
	glfwSetInputMode(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	BlockDatabase::get();

	auto& window = Application::Get().GetWindow();

	float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
	m_camera.setProjectionMatrix(90.f, aspect);

	m_camera.position.z = 3;

	ChunkMeshBuilder builder;
	builder.beginMesh(m_chunk);
	builder.buildMesh();
	builder.endMesh();
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

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_shader.bind();
	m_shader.loadUniformMatrix("u_projView", m_camera.getProjectionViewMatrix());
	m_texture.bind();

	glDrawElements(GL_TRIANGLES, m_chunk.getNumberIndices(), GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
}
