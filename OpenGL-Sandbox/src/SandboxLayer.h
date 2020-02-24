#pragma once

#include<GLCore/Core/Application.h>

#include"OpenGL/Camera.h"
#include"Game/Chunk/Chunk.h"
#include"Game/World.h"

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
	Camera m_camera;
	Chunk m_chunk;
	World m_world;
};