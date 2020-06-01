#include "Database.h"

BlockDatabase::BlockDatabase()
{
	m_blocks.resize(static_cast<unsigned>(ChunkBlock::LAST));
	m_blocks[static_cast<unsigned>(ChunkBlock::air)] = AirBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::grass)] = GrassBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::dirt)] = DirtBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::stone)] = StoneBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::bottom)] = BottomBlock();
}

const BlockData& BlockDatabase::getDataImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData();
}

const Block& BlockDatabase::getBlockImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)];
}