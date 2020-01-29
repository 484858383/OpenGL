#include "VertexArray.h"

#include<GLCore/Core/Log.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	deleteData();
}

void VertexArray::addAttribute(const std::string& attributeName, const std::vector<float>& data, int size)
{
	m_layout.push_back(std::make_pair(attributeName, size));

	glBindVertexArray(m_ID);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(m_indexCounter, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
	glEnableVertexArrayAttrib(m_ID, m_indexCounter++);

	m_buffers.push_back(vbo);
}

void VertexArray::addIndexBuffer(const std::vector<unsigned>& indices)
{
	glBindVertexArray(m_ID);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
	m_numberIndicies = indices.size();

	m_buffers.push_back(ibo);
}

void VertexArray::showLayout() const
{
	LOG_INFO("Displaying vertex array layout for: {}", m_ID);
	LOG_INFO("Name\t\t\t Type");
	for(auto& pair : m_layout)
		LOG_INFO("{}\t\t vec{}", pair.first, pair.second);
}

void VertexArray::deleteData()
{
	glDeleteBuffers(m_buffers.size(), m_buffers.data());
	glDeleteVertexArrays(1, &m_ID);

	m_ID = 0;
	m_indexCounter = 0;
	m_numberIndicies = 0;
	m_buffers.clear();
	m_layout.clear();
}
