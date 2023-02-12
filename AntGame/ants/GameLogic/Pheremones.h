#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "marble/abstraction/UnifiedRenderer.h"
#include "Score.h"

class PheremonesManager {
public:
	struct MazeProperties {

		glm::uvec2 mazeDimensions;
		glm::vec3 startingPoint;
		float wallSize;
		float corridorSpace;
		Maze maze; /*To know nest and food locations*/

	} m_mazeProps;

public:
	PheremonesManager(const MazeProperties& props);
	~PheremonesManager();

	void setPheromones(const std::vector<float>& val) { m_pheromones = val; }
	void setScoreSystem(const ScoreManager *scoreManager) { m_scoreSystem = scoreManager; }

	void step(float delta, const glm::vec3& lookAt);

	void render(const Renderer::Camera& camera);

private:
	void prepareRendering(float delta, const glm::vec3& playerPosition);
	inline glm::vec3 computePheromonePosition(const glm::uvec2& tile);
	inline void drawPheromone(const glm::vec3& position, const glm::vec3& colors, const glm::vec3& size, const glm::vec3& playerPosition);

private:
	const ScoreManager *m_scoreSystem = nullptr;
	std::vector<float>  m_pheromones;

	Renderer::Texture m_texture    = Renderer::Texture("res/textures/pheromones.png");
	Renderer::Texture m_distortion = Renderer::Texture("res/textures/dudvWater.png");
	Renderer::Texture m_normal     = Renderer::Texture("res/textures/waterNormal.png");

	float m_realtime;
	float m_moveFactor = 0;

	struct QuadVertex {

		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;

	};

	struct BatchRenderer {

		static const char *vertexShaderCode, *fragmentShaderCode;

		Renderer::Shader shader{ vertexShaderCode, fragmentShaderCode };

		Renderer::VertexArray m_VAO;
		Renderer::VertexBufferObject m_VBO;
		Renderer::IndexBufferObject m_IBO;

		QuadVertex* m_buffer = nullptr;
		QuadVertex* m_bufferPtr = nullptr;
		unsigned int m_indexCount = 0;

	} m_renderer;

	size_t MaxQuadCount;
	size_t MaxVertexCount;
	size_t MaxIndicesCount;
	size_t MaxTextures;
};

