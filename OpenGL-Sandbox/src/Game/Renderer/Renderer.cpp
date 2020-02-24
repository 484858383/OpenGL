#include "Renderer.h"

#include<GLCore/Core/Application.h>
#include<glfw/include/GLFW/glfw3.h>

#include"../../OpenGL/Camera.h"
#include"../TextureAtlas.h"
#include"..//Chunk/Chunk.h"

Renderer::Renderer()
	:m_chunkShader("TextureVert", "TextureFrag")
{
	glfwSetInputMode(static_cast<GLFWwindow*>(GLCore::Application::Get().GetWindow().GetNativeWindow()),
					 GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::bindCameraImpl(Camera& camera)
{
	m_camera = &camera;
}

void Renderer::addChunkToQueueImpl(const Chunk& chunk)
{
	m_chunks.push_back(&chunk);
}

void Renderer::clearImpl()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::updateImpl()
{
	m_chunkShader.bind();
	m_chunkShader.loadUniformMatrix("u_projView", m_camera->getProjectionViewMatrix());
	TextureAtlas::bind();

	for(const Chunk* chunk : m_chunks)
	{
		chunk->getVertexArray().bind();
		glDrawElements(GL_TRIANGLES, chunk->getNumberIndices(), GL_UNSIGNED_INT, nullptr);
	}

}

void Renderer::initImpl(Camera& camera)
{
	bindCameraImpl(camera);
	auto& window = GLCore::Application::Get().GetWindow();

	float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
	m_camera->setProjectionMatrix(90.f, aspect);
}
