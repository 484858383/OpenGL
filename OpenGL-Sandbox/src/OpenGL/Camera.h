#pragma once

#include<glm/glm.hpp>
#include<GLCore/Core/Timestep.h>

class Camera
{
public:
	Camera() = default;

	void update(GLCore::Timestep ts);
	void input();

	void setProjectionMatrix(float fov, float aspectRatio);
	const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
	glm::mat4 getProjectionViewMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::vec3 lookAt() const;
public:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 velocity = glm::vec3(0.f);
	glm::vec3 size = glm::vec3(0.5f, 1.8f, 0.5f);
	float speed = 1.f;
	bool flying = true;
	bool canJump = true;
private:
	glm::mat4 m_projectionMatrix;
};