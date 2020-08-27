#pragma once

#include<glm/glm/vec2.hpp>
#include<glm/glm/vec3.hpp>

namespace std
{
	template <>
	struct hash<glm::ivec2>
	{
		std::size_t operator()(const glm::ivec2& k) const
		{
			return hash<int>()(static_cast<int>(0.5 * (k.x + k.y) * (k.x + k.y + 1) + k.y));
		}
	};

	template<>
	struct hash<glm::ivec3>
	{
		std::size_t operator()(const glm::ivec3& k) const
		{
			int first = (0.5 * (k.x + k.y) * (k.x + k.y + 1) + k.y);
			return hash<int>()(static_cast<int>(0.5 * (first + k.z) * (first + k.z + 1) + k.z));
		}
	};
}