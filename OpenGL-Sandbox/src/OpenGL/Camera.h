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
	glm::vec3 lookAt() const;
public:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 velocity = glm::vec3(0.f);
private:
	glm::mat4 m_projectionMatrix;
};