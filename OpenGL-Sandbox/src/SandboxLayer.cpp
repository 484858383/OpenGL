#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<GLCore/Core/Input.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>
#include<glm/glm.hpp>

#include<glfw/include/GLFW/glfw3.h>

#include"Game/TextureAtlas.h"

using namespace GLCore;

SandboxLayer::SandboxLayer()
	:m_shader("TextureVert", "TextureFrag"), m_texture("atlas.png")
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

	std::vector<float> vertices =
	{
	//front
	 0, 1, 1,
	 0, 0, 1,
	 1, 0, 1,
	 1, 1, 1,
	 //back
	 1, 1, 0,
	 1, 0, 0,
	 0, 0, 0,
	 0, 1, 0,
	 //left
	 0, 1, 0,
	 0, 0, 0,
	 0, 0, 1,
	 0, 1, 1,
	 //right
	 1, 1, 1,
	 1, 0, 1,
	 1, 0, 0,
	 1, 1, 0,
	 //top
	 1, 1, 1,
	 1, 1, 0,
	 0, 1, 0,
	 0, 1, 1,
	 //bottom
	 1, 0, 1,
	 1, 0, 0,
	 0, 0, 0,
	 0, 0, 1,
	};

	std::vector<float> textureCoords;

	auto t1 = TextureAtlas::getUVCoordinates(1, 0);
	auto t2 = TextureAtlas::getUVCoordinates(2, 0);
	auto t3 = TextureAtlas::getUVCoordinates(3, 0);


	for(int i = 0; i < 4; i++)
	textureCoords.insert(textureCoords.end(), t2.begin(), t2.end());

	textureCoords.insert(textureCoords.end(), t1.begin(), t1.end());
	textureCoords.insert(textureCoords.end(), t3.begin(), t3.end());

	std::vector<unsigned> indicies =
	{
	 0, 1, 2,
	 2, 3, 0,
	 
	 4, 5, 6,
	 6, 7, 4,
	 
	 8, 9, 10,
	 10, 11, 8,
	 
	 12, 13, 14,
	 14, 15, 12,
	 
	 16, 17, 18,
	 18, 19, 16,
	 
	 20, 21, 22,
	 22, 23, 20
	};

	m_va.addAttribute("a_position", vertices, 3);
	m_va.addAttribute("a_texCoords", textureCoords, 2);
	m_va.addIndexBuffer(indicies);

	auto& window = Application::Get().GetWindow();

	float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
	m_camera.setProjectionMatrix(90.f, aspect);

	m_camera.position.z = 3;
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

	glDrawElements(GL_TRIANGLES, m_va.getNumberIndicies(), GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
}
