#include "ChunkManager.h"

ChunkManager::ChunkManager(World& world)
	:m_world(&world)
{
}

void ChunkManager::add(glm::ivec2&& key, Chunk&& chunk)
{
	m_chunks.emplace(std::move(key), std::move(chunk));
}

void ChunkManager::prepareChunkToBuild(const glm::ivec2& position)
{
	if(!chunkExistsAt(position))
		return;
	m_keysToBuild.insert(position);
}

void ChunkManager::prepareChunkToDelete(const glm::ivec2& position)
{
	if(!chunkExistsAt(position))
		return;
	m_keysToDelete.insert(position);
}

void ChunkManager::deleteChunks()
{
	for(auto& key : m_keysToDelete)
		m_chunks.erase(key);

	m_keysToDelete.clear();
}

void ChunkManager::buildChunks()
{
	/*for(auto key : m_keysToBuild)
	{
		if(chunkExistsAt(key))
		{
			m_chunkBuilder.beginMesh(m_chunks.at(key), *m_world);
			m_chunkBuilder.buildMesh();
			m_chunkBuilder.endMesh();
		}
	}
	m_keysToBuild.clear();*/

	auto iter = m_keysToBuild.begin();
	if(iter != m_keysToBuild.end())
	{
		if(chunkExistsAt(*iter))
		{
			m_chunkBuilder.beginMesh(m_chunks.at(*iter), *m_world);
			m_chunkBuilder.buildMesh();
			m_chunkBuilder.endMesh();
		}
		m_keysToBuild.erase(iter);
	}
}

bool ChunkManager::chunkExistsAt(const glm::ivec2& position) const
{
	return m_chunks.find(position) != m_chunks.end();
}
