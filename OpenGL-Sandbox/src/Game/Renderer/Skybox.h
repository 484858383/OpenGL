#pragma once

#include"../../OpenGL/Cubemap.h"
#include"../../OpenGL/Buffer/VertexArray.h"

class Skybox
{
public:
	Skybox(const std::string& filename);

	const Cubemap& getCubemap() const { return m_cubemap; }
	const VertexArray& getCubeMesh() const { return m_cubeMesh; }
private:
	Cubemap m_cubemap;
	VertexArray m_cubeMesh;
};