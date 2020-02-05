#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

using namespace GLCore;

SandboxLayer::SandboxLayer()
	:m_shader("TextureVert", "TextureFrag"), m_texture("test.png")
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	Utils::EnableGLDebugging();

	std::vector<float> vertices =
	{
	 -0.5f,  0.5f, 1.f,
	 -0.5f, -0.5f, 1.f,
	  0.5f, -0.5f, 1.f,
	  0.5f,  0.5f, 1.f,
	};

	std::vector<float> textureCoords =
	{
	 0.f, 1.f,
	 0.f, 0.f,
	 1.f, 0.f,
	 1.f, 1.f
	};

	std::vector<unsigned> indicies =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_va.addAttribute("a_position", vertices, 3);
	m_va.addAttribute("a_texCoords", textureCoords, 2);
	m_va.addIndexBuffer(indicies);

	auto& window = Application::Get().GetWindow();

	float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
	m_camera.setProjectionMatrix(90.f, aspect);
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	m_camera.position.z = 3;
	glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3(1, 0, 0));


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	m_shader.bind();
	m_shader.loadUniformMatrix("u_projView", m_camera.getProjectionViewMatrix() * model);
	m_texture.bind();

	glDrawElements(GL_TRIANGLES, m_va.getNumberIndicies(), GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
}
