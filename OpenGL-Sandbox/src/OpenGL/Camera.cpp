#include "Camera.h"

#include<glm/gtc/matrix_transform.hpp>
#include<glfw/include/GLFW/glfw3.h>
#include<GLCore/Core/Application.h>
#include<GLCore/Core/Input.h>


void Camera::update(GLCore::Timestep ts)
{
	position += velocity * ts.GetSeconds();
	velocity *= 0.95f;
}

void Camera::input()
{
	if(GLCore::Input::IsKeyPressed(GLFW_KEY_W))
	{
		velocity.z -= speed * glm::sin(glm::radians(rotation.x + 90));
		velocity.x -= speed * glm::cos(glm::radians(rotation.x + 90));
	}
	if(GLCore::Input::IsKeyPressed(GLFW_KEY_A))
	{
		velocity.z -= speed * glm::sin(glm::radians(rotation.x));
		velocity.x -= speed * glm::cos(glm::radians(rotation.x));
	}
	if(GLCore::Input::IsKeyPressed(GLFW_KEY_S))
	{
		velocity.z += speed * glm::sin(glm::radians(rotation.x + 90));
		velocity.x += speed * glm::cos(glm::radians(rotation.x + 90));
	}
	if(GLCore::Input::IsKeyPressed(GLFW_KEY_D))
	{
		velocity.z += speed * glm::sin(glm::radians(rotation.x));
		velocity.x += speed * glm::cos(glm::radians(rotation.x));
	}

	if(GLCore::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || GLCore::Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		velocity.y -= speed;
	}
	if(GLCore::Input::IsKeyPressed(GLFW_KEY_SPACE))
	{
		velocity.y += speed;
	}

	auto& window = GLCore::Application::Get().GetWindow();

	static glm::vec2 lastMousePos(window.GetWidth() / 2.f, window.GetHeight() / 2.f);
	glm::vec2 mousePos(GLCore::Input::GetMouseX(), GLCore::Input::GetMouseY());

	glm::vec2 delta;
	delta.x = mousePos.x - lastMousePos.x;
	delta.y = lastMousePos.y - mousePos.y;

	delta *= 0.1f;

	rotation.x += delta.x;
	rotation.y -= delta.y;

	lastMousePos = {GLCore::Input::GetMouseX(), GLCore::Input::GetMouseY()};

	if(rotation.y < -95.f)
		rotation.y = -95.f;
	if(rotation.y > 95.f)
		rotation.y = 95.f;
}

void Camera::setProjectionMatrix(float fov, float aspectRatio)
{
	m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.f);
}

glm::mat4 Camera::getProjectionViewMatrix() const
{
	glm::mat4 view(1.0f);
	view = glm::rotate(view, glm::radians(rotation.y), {1.f, 0.f, 0.f});
	view = glm::rotate(view, glm::radians(rotation.x), {0.f, 1.f, 0.f});
	view = glm::translate(view, -position);

	return m_projectionMatrix * view;
}

glm::mat4 Camera::getViewMatrix() const
{
	glm::mat4 view(1.0f);
	view = glm::rotate(view, glm::radians(rotation.y), {1.f, 0.f, 0.f});
	view = glm::rotate(view, glm::radians(rotation.x), {0.f, 1.f, 0.f});
	return glm::translate(view, -position);
}

glm::vec3 Camera::lookAt() const
{
	glm::mat4 view(1.0f);
	view = glm::rotate(view, glm::radians(rotation.y), {1.f, 0.f, 0.f});
	view = glm::rotate(view, glm::radians(rotation.x), {0.f, 1.f, 0.f});
	view = glm::translate(view, -position);

	view = glm::inverse(view);

	return -normalize(glm::vec3(view[2]));
}
