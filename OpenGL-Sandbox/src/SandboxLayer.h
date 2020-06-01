#pragma once

#include<GLCore/Core/Application.h>

#include"OpenGL/Camera.h"
#include"Game/World/World.h"
#include"OpenGL/Buffer/VertexArray.h"
#include"OpenGL/Model2D.h"
#include"OpenGL/Texture.h"

class Clock;

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
	void raycast(Clock& clock);
private:
	Camera m_camera;
	World m_world;

	Model2D m_crosshair;
	std::unordered_map<std::string, Texture> m_textures;
private:
	float m_timeToBreak = 0.25f; //time between breaking blocks when holding mouse1
	float m_rayStep = 0.02f; //the lower this value is the more accurate it will be but it will take longer
	float m_blockBreakRange = 5.f; //unit is number of blocks
};