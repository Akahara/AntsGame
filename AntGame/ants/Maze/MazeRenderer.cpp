#include "MazeRenderer.h"

struct MeshInstance {
	glm::vec3 position;
	glm::vec3 scale;
	float     rotation;

	static Renderer::VertexBufferLayout getLayout()
	{
		return Renderer::VertexBufferLayout()
			.push<float>(3)
			.push<float>(3)
			.push<float>(1)
			;
	}
};

MazeRenderer::MazeRenderer()
{
	m_shader = Renderer::ShaderFactory()
		.prefix("res/shaders/")
		.addFileVertex("instanced.vs")
		.prefix("mesh_parts/")
		.addFileFragment("base.fs")
		.addFileFragment("color_singletexture.fs")
		.addFileFragment("lights_pointlights.fs")
		.addFileFragment("final_fog.fs")
		.addFileFragment("shadows_normal.fs")
		.addFileFragment("normal_normalmap.fs").build();
	m_wallMesh = Renderer::loadMeshFromFile("res/meshes/wall.obj");
	m_pillarMesh = Renderer::loadMeshFromFile("res/meshes/pillar.obj");
}

void MazeRenderer::buildMaze(const Maze *maze)
{
	m_maze = maze;
	buildPillars();
	buildWalls();
	buildDecals();
}

void MazeRenderer::render(const Renderer::Camera &camera)
{
	m_shader.bind();
	m_shader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
	m_pillarMesh.draw((int)m_pillarCount);
	m_wallMesh.draw((int)m_wallCount);
	m_shader.unbind();
}

void MazeRenderer::buildPillars()
{
	const Maze &maze = *m_maze;
	m_pillarCount = ((size_t)maze.nbLine + 1) * ((size_t)maze.nbColumn + 1);
	MeshInstance *pillarInstances = new MeshInstance[m_pillarCount];

	constexpr float W = MazeMeshGenerator::WALL_SIZE;
	constexpr float C = MazeMeshGenerator::CORRIDOR_SPACE;
	constexpr float H = MazeMeshGenerator::WALL_HEIGHT;

	for (size_t x = 0; x <= maze.nbColumn; x++) {
		for (size_t y = 0; y <= maze.nbLine; y++) {
			glm::vec3 position = { x * C, 0, y * C };
			glm::vec3 scale = { W * .5f, H, W * .5f };
			pillarInstances[x + y * ((size_t)maze.nbColumn + 1)] = { position, scale, 0.f };
		}
	}

	m_pillarsInstanceBuffer = Renderer::VertexBufferObject{ pillarInstances, sizeof(MeshInstance) * m_pillarCount };
	Renderer::VertexBufferLayout instanceLayout = MeshInstance::getLayout();
	const Renderer::VertexBufferLayout &verticesLayout = Renderer::Vertex::getVertexBufferLayout();
	m_pillarMesh.getVAO().addInstanceBuffer(m_pillarsInstanceBuffer, instanceLayout, verticesLayout);

	delete[] pillarInstances;
}

void MazeRenderer::buildWalls()
{
	const Maze &maze = *m_maze;
	m_wallCount = 0;
	for (size_t x = 0; x < maze.nbLine; x++) {
		for (size_t y = 0; y < maze.nbColumn; y++) {
			unsigned int tile = maze.tiles[x + y * maze.nbColumn];
			static_assert(MazeMeshGenerator::EAST == (1 << 1));
			static_assert(MazeMeshGenerator::NORTH == (1 << 0));
			m_wallCount += (tile >> 1) & 1;
			m_wallCount += (tile >> 0) & 1;
		}
	}
	m_wallCount += (size_t)maze.nbLine + maze.nbColumn;

	MeshInstance *wallInstances = new MeshInstance[m_wallCount];

	constexpr float W = MazeMeshGenerator::WALL_SIZE;
	constexpr float C = MazeMeshGenerator::CORRIDOR_SPACE;
	constexpr float H = MazeMeshGenerator::WALL_HEIGHT;

	size_t c = 0;
	for (size_t i = 0; i < maze.nbColumn; i++)
		wallInstances[c++] = { { 0,0,C * .5f + C * i }, { W,H,(C - W) * .5f }, 0 };
	for (size_t i = 0; i < maze.nbLine; i++)
		wallInstances[c++] = { { C * .5f + C * i,0,0 }, { (C - W) * .5f,H,W }, 1 };
	for (size_t x = 0; x < maze.nbLine; x++) {
		for (size_t y = 0; y < maze.nbColumn; y++) {
			if (m_maze->tiles[x + y * maze.nbColumn] & MazeMeshGenerator::EAST)
				wallInstances[c++] = { { C * (x + 1),0,C * .5f + C * y }, { W,H,(C - W) * .5f }, 0 };
			if (m_maze->tiles[x + y * maze.nbColumn] & MazeMeshGenerator::SOUTH)
				wallInstances[c++] = { { C * .5f + C * x,0,C * (y + 1) }, { (C - W) * .5f,H,W }, 1 };
		}
	}

	m_wallsInstanceBuffer = Renderer::VertexBufferObject{ wallInstances, sizeof(MeshInstance) * m_wallCount };
	Renderer::VertexBufferLayout instanceLayout = MeshInstance::getLayout();
	const Renderer::VertexBufferLayout &verticesLayout = Renderer::Vertex::getVertexBufferLayout();
	m_wallMesh.getVAO().addInstanceBuffer(m_wallsInstanceBuffer, instanceLayout, verticesLayout);

	m_wallCount = c;

	delete[] wallInstances;
}

void MazeRenderer::buildDecals()
{
}
