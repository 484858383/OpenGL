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
	static const Block& getBlock(ChunkBlock block) { return get().getBlockImpl(block); }
private:
	BlockDatabase();

	const BlockData& getDataImpl(ChunkBlock block) const;
	const Block& getBlockImpl(ChunkBlock block) const;
private:
	std::vector<Block> m_blocks;
};