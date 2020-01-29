#pragma once

#include<glad/glad.h>
#include<vector>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addAttribute(const std::string& attributeName, const std::vector<float>& data, int size);
	///attributeName is the name you use in the shader, e.g. "a_position"
	///data is the vertex data
	///size is the number of componets in the vector, e.g. vec3 means size = 3

	void addIndexBuffer(const std::vector<unsigned>& indices);
	void showLayout() const;

	void bind() const { glBindVertexArray(m_ID); }
	void unbind() const { glBindVertexArray(0); }
	GLuint getID() const { return m_ID; }
	unsigned getNumberIndicies() const { return m_numberIndicies; }
private:
	void deleteData();
private:
	GLuint m_ID;
	unsigned m_numberIndicies = 0;
	unsigned m_indexCounter = 0;

	std::vector<GLuint> m_buffers;
	std::vector<std::pair<std::string, int>> m_layout;
};