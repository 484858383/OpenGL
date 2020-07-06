#pragma once

#include<vector>

struct Mesh
{
	std::vector<float> positions;
	std::vector<float> textureCoords;
	std::vector<unsigned> indices;
	std::vector<float> faceBrightness;

	void clear() 
	{
		positions.clear(); 
		textureCoords.clear();
		indices.clear(); 
		faceBrightness.clear();

		positions.shrink_to_fit(); 
		textureCoords.shrink_to_fit(); 
		indices.shrink_to_fit();
		faceBrightness.shrink_to_fit();
	}
};