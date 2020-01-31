#pragma once

#include<GLCore/Core/Application.h>

#include"OpenGL/Shader/Shader.h"
#include"OpenGL/Buffer/VertexArray.h"
#include"OpenGL/Texture.h"

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	Shader m_shader;
	VertexArray m_va;
	Texture m_texture;
};