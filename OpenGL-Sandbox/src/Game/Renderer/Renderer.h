#pragma once

#include"../../OpenGL/Shader/Shader.h"

class Chunk;
class Camera;

class Renderer
{
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	~Renderer() = default;

	static Renderer& get() { static Renderer r; return r; }

	static void bindCamera(Camera& camera) { get().bindCameraImpl(camera); }
	static void addChunkToQueue(const Chunk& chunk) { get().addChunkToQueueImpl(chunk); }
	static void clear() { get().clearImpl(); }
	static void update() { get().updateImpl(); }
	static void init(Camera& camera) { get().initImpl(camera); }
private:
	Renderer();

	void bindCameraImpl(Camera& camera);
	void addChunkToQueueImpl(const Chunk& chunk);
	void clearImpl();
	void updateImpl();
	void initImpl(Camera& camera);
private:
	Shader m_chunkShader;
	Camera* m_camera = nullptr;
	std::vector<const Chunk*> m_chunks;
};