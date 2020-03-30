#pragma once

#include<glm/vec3.hpp>
#include<glm/geometric.hpp>
#include"../../OpenGL/Camera.h"

class Ray
{
public:
	Ray(const Camera& camera) { m_start = m_current = camera.position; m_direction = camera.lookAt(); }

	void step(float size) { m_current += size * m_direction; }
	float length() { return glm::distance(m_start, m_current); }

	const glm::vec3& getPosition() const { return m_current; }
private:
	glm::vec3 m_start;
	glm::vec3 m_current;
	glm::vec3 m_direction;
};