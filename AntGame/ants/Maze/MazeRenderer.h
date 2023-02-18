#pragma once

#include "marble/abstraction/VertexBufferObject.h"
#include "marble/abstraction/Mesh.h"
#include "marble/abstraction/Shader.h"
#include "marble/abstraction/UnifiedRenderer.h"
#include "MazeMeshGenerator.h"

class MazeRenderer {
private:
	const Maze      *m_maze;
	Renderer::Shader m_shader;
	Renderer::Mesh   m_wallMesh;
	Renderer::Mesh   m_pillarMesh;
	size_t           m_wallCount;
	size_t           m_pillarCount;
	Renderer::VertexBufferObject m_wallsInstanceBuffer;
	Renderer::VertexBufferObject m_pillarsInstanceBuffer;

public:
	MazeRenderer();

	void buildMaze(const Maze *maze);
	void render(const Renderer::Camera &camera);

private:
	void buildPillars();
	void buildWalls();
	void buildDecals();
};