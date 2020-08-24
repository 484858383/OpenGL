#pragma once

#include<glm/vec3.hpp>

//position is the position of the bottom left back vertex of the bounding box
//for a unit box (cube): position = {0, 0, 0}, size = {1, 1, 1}

struct AABB
{
	//other classes
	enum class CollisionDirection
	{
		none, x, y, z
	};

	struct CollisionData
	{
		bool collision = false;
		float distance = 0.f;
		CollisionDirection dir = CollisionDirection::none;
	};

	//AABB
	glm::vec3 position;
	glm::vec3 size;
	glm::vec3 minExtent;
	glm::vec3 maxExtent;

	AABB(const glm::vec3& position, const glm::vec3& size)
		:position(position), size(size)
	{
		minExtent = position;
		maxExtent = position + size;
	}

	CollisionData intersects(const AABB& other)
	{
		CollisionDirection dir = CollisionDirection::none;

		glm::vec3 distances1 = other.position - (size + position);
		glm::vec3 distances2 = position - (other.size + other.position);
		glm::vec3 distances = glm::max(distances1, distances2);

		float maxDistance = glm::max(glm::max(distances.x, distances.y), distances.z);

		if(maxDistance == distances.x) dir = CollisionDirection::x;
		else if(maxDistance == distances.y) dir = CollisionDirection::y;
		else if(maxDistance == distances.z) dir = CollisionDirection::z;

		return {maxDistance < 0.f, maxDistance, dir};
	}

	bool contains(const glm::vec3& point)
	{
		return point.x >= position.x && point.x <= (position.x + size.x)
			&& point.y >= position.y && point.y <= (position.y + size.y)
			&& point.z >= position.z && point.z <= (position.z + size.z);
	}
};
