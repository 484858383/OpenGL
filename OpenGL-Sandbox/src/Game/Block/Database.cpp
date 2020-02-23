#include "Database.h"

BlockDatabase::BlockDatabase()
{
	m_blocks.resize(static_cast<unsigned>(ChunkBlock::LAST));
	m_blocks[static_cast<unsigned>(ChunkBlock::air)] = AirBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::grass)] = GrassBlock();
}

const BlockData& BlockDatabase::getDataImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData();
}

const Block& BlockDatabase::getBlockImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)];
}