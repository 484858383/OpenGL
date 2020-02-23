#pragma once

enum class ChunkBlock
{
	air = 0,
	grass,

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