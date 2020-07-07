#include "Skybox.h"

namespace
{
	std::vector<float> skyboxVertices = {
		// positions          
		-500.0f,  500.0f, -500.0f,
		-500.0f, -500.0f, -500.0f,
		 500.0f, -500.0f, -500.0f,
		 500.0f,  500.0f, -500.0f,

		-500.0f, -500.0f,  500.0f,
		-500.0f, -500.0f, -500.0f,
		-500.0f,  500.0f, -500.0f,
		-500.0f,  500.0f,  500.0f,

		 500.0f, -500.0f, -500.0f,
		 500.0f, -500.0f,  500.0f,
		 500.0f,  500.0f,  500.0f,
		 500.0f,  500.0f, -500.0f,

		-500.0f, -500.0f,  500.0f,
		-500.0f,  500.0f,  500.0f,
		 500.0f,  500.0f,  500.0f,
		 500.0f, -500.0f,  500.0f,

		-500.0f,  500.0f, -500.0f,
		 500.0f,  500.0f, -500.0f,
		 500.0f,  500.0f,  500.0f,
		-500.0f,  500.0f,  500.0f,

		-500.0f, -500.0f, -500.0f, //--- 0
		-500.0f, -500.0f,  500.0f, //--+ 1
		 500.0f, -500.0f, -500.0f, //+-- 2
	  // 500.0f, -500.0f, -500.0f, //+-- 2
	  //-500.0f, -500.0f,  500.0f, //--+ 1
		 500.0f, -500.0f,  500.0f  //+-+ 3
	};

	std::vector<unsigned> indices =
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		//different because the data im using for the cube vertices is strange for the last face
		20, 21, 22,
		22, 21, 23
	};
}


Skybox::Skybox(const std::string& filename)
	:m_cubemap(filename)
{
	m_cubeMesh.addAttribute("a_position", skyboxVertices, 3);
	m_cubeMesh.addIndexBuffer(indices);
}