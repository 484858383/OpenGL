#pragma once

enum class ChunkBlock
{
	air = 0,
	grass,
	dirt,
	stone,
	bottom,
	sand = 5,
	water,
	oak_log,
	oak_leaves,
	cactus,
	snow = 10,

	LAST
};

inline bool operator==(const Block& lhs, ChunkBlock rhs)
{
	return lhs.getID() == static_cast<int>(rhs);
}

inline bool operator!=(const Block& lhs, ChunkBlock rhs)
{
	return !(lhs == rhs);
}