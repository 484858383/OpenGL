#pragma once

#include<vector>

#include"Block.h"
#include"ChunkBlock.h"

class BlockDatabase
{
public:
	BlockDatabase(const BlockDatabase&) = delete;
	BlockDatabase& operator=(const BlockDatabase&) = delete;

	BlockDatabase(BlockDatabase&&) = delete;
	BlockDatabase& operator=(BlockDatabase&&) = delete;

	static BlockDatabase& get() { static BlockDatabase bd; return bd; }

	static const BlockData& getData(ChunkBlock block) { return get().getDataImpl(block); }
	static const glm::ivec2& getTextureCoords(ChunkBlock block) { return get().getTextureCoordsImpl(block); }
	static bool blockIsTransparent(ChunkBlock block) { return get().blockIsTransparentImpl(block); }
	static bool blockIsFoliage(ChunkBlock block) { return get().blockIsFoliageImpl(block); }
private:
	BlockDatabase();

	const BlockData& getDataImpl(ChunkBlock block) const;
	const glm::ivec2& getTextureCoordsImpl(ChunkBlock block) const;
	bool blockIsTransparentImpl(ChunkBlock block) const;
	bool blockIsFoliageImpl(ChunkBlock block) const;
private:
	std::vector<Block> m_blocks;
};