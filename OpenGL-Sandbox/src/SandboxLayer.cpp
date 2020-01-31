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
	 -0.5f,  0.5f, 
	 -0.5f, -0.5f,
	  0.5f, -0.5f,
	  0.5f,  0.5f
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

	m_va.addAttribute("a_position", vertices, 2);
	m_va.addAttribute("a_texCoords", textureCoords, 2);
	m_va.addIndexBuffer(indicies);
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
	glm::mat4 transform = glm::translate(glm::mat4(1), {0.5f, 0.5f, 0.f});


	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.bind();
	m_shader.loadUniformMatrix("u_transform", transform);
	m_texture.bind();

	glDrawElements(GL_TRIANGLES, m_va.getNumberIndicies(), GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
}
