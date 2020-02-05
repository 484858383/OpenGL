#include "Camera.h"

#include<glm/gtc/matrix_transform.hpp>

void Camera::update(GLCore::Timestep ts)
{
	position += velocity * ts.GetSeconds();

}

void Camera::input()
{
}

void Camera::setProjectionMatrix(float fov, float aspectRatio)
{
	m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.f);
}

glm::mat4 Camera::getProjectionViewMatrix() const
{
	glm::mat4 view(1.f);
	view = glm::rotate(view, rotation.x, {0.f, 1.f, 0.f});
	view = glm::rotate(view, rotation.y, {1.f, 0.f, 0.f});
	view = glm::translate(view, -position);

	return m_projectionMatrix * view;
}
