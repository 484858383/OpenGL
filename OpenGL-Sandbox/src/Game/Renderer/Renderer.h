#pragma once

#include"../../OpenGL/Shader/Shader.h"
#include"../../Clock.h"
#include"Skybox.h"

class Chunk;
class Camera;
class Model2D;

class Renderer
{
public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	~Renderer() = default;

	static Renderer& get() { static Renderer r; return r; }

	static void addChunkToQueue(const Chunk& chunk) { get().addChunkToQueueImpl(chunk); }
	static void add2DModelToQueue(const Model2D& model) { get().add2DModelToQueueImpl(model); }

	static void bindCamera(Camera& camera) { get().bindCameraImpl(camera); }
	static void clear() { get().clearImpl(); }
	static void update(float time) { get().updateImpl(time); }
	static void init(Camera& camera) { get().initImpl(camera); }
private:
	Renderer();

	void addChunkToQueueImpl(const Chunk& chunk);
	void add2DModelToQueueImpl(const Model2D& model);

	void bindCameraImpl(Camera& camera);
	void clearImpl();
	void updateImpl(float time);
	void initImpl(Camera& camera);

	void drawChunks(float time);
	void drawSkybox(float time);
	void draw2DModels(float time);
private:
	//shaders
	Shader m_chunkShader;
	Shader m_2dTextureShader;
	Shader m_waterShader;
	Shader m_skyboxShader;
private:
	//tools
	Camera* m_camera = nullptr;
	Clock m_clock;
private:
	//models/vertex arrays
	Skybox m_skybox;
	std::vector<const Chunk*> m_chunks;
	std::vector<const Model2D*> m_2dModels;
};