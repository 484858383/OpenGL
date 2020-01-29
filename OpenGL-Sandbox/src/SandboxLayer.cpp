#include "SandboxLayer.h"

#include<GLCore/Util/OpenGLDebug.h>
#include<imgui.h>

using namespace GLCore;

SandboxLayer::SandboxLayer()
	:m_shader("FlatColorVert", "FlatColorFrag")
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

	std::vector<unsigned> indicies =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_va.addAttribute("a_position", vertices, 2);
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
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.bind();
	m_shader.loadUniform("u_color", m_uniformColor.x, m_uniformColor.y, m_uniformColor.z);

	glDrawElements(GL_TRIANGLES, m_va.getNumberIndicies(), GL_UNSIGNED_INT, nullptr);
}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("test");

	ImGui::ColorEdit3("clear color", (float*)&m_uniformColor);

	ImGui::End();
}
