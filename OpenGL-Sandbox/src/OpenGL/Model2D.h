#pragma once

#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>
#include"Buffer/VertexArray.h"
#include"Mesh.h"
#include"Texture.h"

class Model2D
{
public:
	Model2D() = default;

	Model2D(const Mesh& mesh, Texture& texture)
		:m_texture(&texture)
	{
		constructMesh(mesh, texture);
	}

	void constructMesh(const Mesh& mesh, Texture& texture)
	{
		m_texture = &texture;
		m_model.addAttributes(mesh);
		m_readyToRender = true;
	}

	Model2D(const Model2D&) = delete;
	Model2D& operator=(const Model2D&) = delete;

	Model2D(Model2D&&) = default;
	Model2D& operator=(Model2D&&) = default;

	~Model2D() = default;

	const VertexArray& getVertexArray() const { return m_model; }

	void bind() const { m_model.bind(); m_texture->bind(); }
	void unbind() const { m_model.unbind(); m_texture->unbind(); }
	unsigned getNumberIndicies() const { return m_model.getNumberIndicies(); }

	glm::mat4 getTransform() const
	{
		glm::mat4 ret = glm::mat4(1.f);
		ret = glm::translate(ret, {position.x, position.y, 0.f}) * glm::rotate(ret, glm::radians(rotation), {0.f, 0.f, 1.f})
			* glm::scale(ret, {scale.x, scale.y, 1.f});
		return ret;
	}

	glm::vec2 position = {0.f, 0.f};
	glm::vec2 scale = {1.f, 1.f};
	float rotation = 0.f;
private:
	bool m_readyToRender = false;
	VertexArray m_model;
	Texture* m_texture;
};