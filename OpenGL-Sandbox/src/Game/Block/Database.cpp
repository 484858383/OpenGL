#include "Database.h"

BlockDatabase::BlockDatabase()
{
	m_blocks.reserve(static_cast<unsigned>(ChunkBlock::LAST));
	m_blocks[static_cast<unsigned>(ChunkBlock::air)] = AirBlock();
	m_blocks[static_cast<unsigned>(ChunkBlock::grass)] = GrassBlock();
}

const BlockData& BlockDatabase::getDataImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData();
}

const glm::ivec2& BlockDatabase::getTextureCoordsImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData().textureCoords;
}

bool BlockDatabase::blockIsTransparentImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData().isTransparent;
}

bool BlockDatabase::blockIsFoliageImpl(ChunkBlock block) const
{
	return m_blocks[static_cast<unsigned>(block)].getData().isFoliage;
}
