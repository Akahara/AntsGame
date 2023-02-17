#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <thread>

#include "Scene.h"

#include "marble/World/Player.h"
#include "marble/World/Sky.h"
#include "marble/World/TerrainGeneration/Terrain.h"
#include "marble/World/TerrainGeneration/Noise.h"
#include "marble/Utils/Debug.h"

#include "../Maze/MazeMeshGenerator.h"
#include "../Maze/FoodMeshGenerator.h"
#include "../Maze/MazeGeneration_proj/libAntMaze.h"
#include "../Maze/MazeTileSystem.h"
#include "../GameLogic/AntsPlayer.h"
#include "../GameLogic/Score.h"
#include "../GameLogic/Pheremones.h"
#include "../Client/client.h"

#include "marble/abstraction/pipeline/VFXPipeline.h"
#include "marble/World/Props/PropsManager.h"

#include "../GameLogic/Food.h";

static Server::Client::MOVE_LIST computeMoveDirection(const glm::uvec2& old_tile, const glm::uvec2& new_tile) {
	glm::ivec2 delta = old_tile - new_tile;
	// delta y = -1 --> up 0 (0, 1)
	// delta y = 1 --> down 1 (0, -1)
	// delta x = 1 --> left 2 (1, 0)
	// delta x = -1 --> right 3 (-1, 0)

	switch (delta.y) {
		case 0:
			break;
		case 1:
			return Server::Client::MOVE_LIST::UP;

		case -1:
			return Server::Client::MOVE_LIST::DOWN;
	}

	switch (delta.x) {
		case 1:
			return Server::Client::MOVE_LIST::LEFT;

		case -1:
			return Server::Client::MOVE_LIST::RIGHT;
	}
	return Server::Client::MOVE_LIST::WTF; // how
}

class Playground : public Scene {
private:
	World::Sky m_sky{ World::Sky::SkyboxesType::SAND };

	float m_realTime = 0;

	bool       m_useDbgPlayer = false;
	AntsPlayer m_player;
	Player     m_debugPlayer;
	Maze       m_maze;
	ScoreManager m_score;

	Terrain::Terrain  m_terrain;
	Renderer::Mesh    m_mazeMesh;
	Renderer::Texture m_sandTexture = Renderer::Texture("res/textures/sand1.jpg");
	Renderer::Mesh    m_wallMesh;

	visualEffects::VFXPipeline m_pipeline;

	PheremonesManager* m_pManager;
	World::PropsManager m_props;

	//FoodLogic* m_foodLogic;

	Server::Client* m_client = nullptr; // might change if global state idk

public:

	Playground() 
	{
		// Local server creation
		if (!m_client) {
			/* TODO : fix the crash when quitting the application */
			std::string adress = "127.0.0.1";
			unsigned short port = 8080;

			// Client client1{ io_context1, adress, port };
			m_client = new Server::Client(adress, port);

			m_client->join(1);

			while (!m_client->getMaze().has_value()) {
				m_client->listenClient();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}

		// Game Logic setup (maze, food, score, pheromones)
		{
			m_maze = Maze{
				m_client->getMaze()->nbColumn,
				m_client->getMaze()->nbLine,
				m_client->getMaze()->nestColumn,
				m_client->getMaze()->nestLine,
				m_client->getMaze()->tiles
			};

			m_player.setMaze(m_maze);
			//m_foodLogic = new FoodLogic(m_maze);
			m_pManager = new PheremonesManager(
				PheremonesManager::MazeProperties{
				glm::uvec2(m_client->getMaze()->nbColumn,m_client->getMaze()->nbLine),
				glm::vec3(0,0,0),
				5.f,20.f,
				*m_client->getMaze()
				}
			);

			m_score.setMaze(m_maze);
			m_pManager->scoreSystem = &m_score;
		}

		// Mesh and terration generation
		{
			m_mazeMesh = MazeMeshGenerator::generateMazeMesh(m_maze);
			std::vector<World::Prop> m_fsources = MazeMeshGenerator::generateFoodSources(m_maze);
			std::for_each(m_fsources.begin(), m_fsources.end(), [&](const World::Prop& source) {m_props.feed(source); });
			generateTerrain();
			m_pipeline.registerEffect<visualEffects::Bloom>();
			m_wallMesh = Renderer::loadMeshFromFile("res/meshes/wall.obj");
		}

		// Shader Factory
		{
			int samplers[8] = { 0,1,2,3,4,5,6,7 };
			Renderer::Shader& meshShader = Renderer::rebuildStandardMeshShader(Renderer::ShaderFactory()
				.prefix("res/shaders/")
				.addFileVertex("standard.vs")
				.prefix("mesh_parts/")
				.addFileFragment("base.fs")
				.addFileFragment("color_singletexture.fs")
				.addFileFragment("lights_pointlights.fs")
				.addFileFragment("final_fog.fs")
				.addFileFragment("shadows_normal.fs")
				.addFileFragment("normal_normalmap.fs"));
			meshShader.bind();
			meshShader.setUniform1f("u_Strength", 1.25f);
			meshShader.setUniform1iv("u_Textures2D", 8, samplers);
			meshShader.setUniform3f("u_fogDamping", .002f, .002f, .004f);
			meshShader.setUniform3f("u_fogColor", 1.000f, 0.944f, 0.102f);
			meshShader.setUniform3f("u_SunPos", 1000, 1000, 1000);
			Renderer::Shader::unbind();
		}
	}

	~Playground() {
		delete m_pManager;
	}

	void generateTerrain()
	{
		// 20x20 * 20x20 ~= 512x512 which is a good size of the erosion algorithm
		constexpr unsigned int chunkSize = 10;
		constexpr unsigned int chunkCount = 10;

		{ // simple terrain + erosion
			unsigned int noiseMapSize = 3 + chunkSize * chunkCount;
			float* noiseMap = Noise::generateNoiseMap(noiseMapSize,
				noiseMapSize,
				/*scale*/30,
				/*octaves*/4,
				/*persistence*/.5f,
				/*frequency*/1.f,
				/*lacunarity*/2.1f,
				/*seed*/0);
			Noise::rescaleNoiseMap(noiseMap, noiseMapSize, noiseMapSize, 0, 1, 0, /*terrain height*/2.f);
			Terrain::HeightMap* heightMap = new Terrain::ConcreteHeightMap(noiseMapSize, noiseMapSize, noiseMap);
			m_terrain = Terrain::generateTerrain(heightMap, chunkCount, chunkCount, chunkSize);
		}
	}

	void step(float delta) override
	{
		m_realTime += delta;

		
		// Tile checks
		glm::vec3 playerPos = m_player.getPosition();
		glm::uvec2 pos = MazeTileSystem::getTileCoordinates(playerPos,
			glm::uvec2(m_maze.nbColumn, m_maze.nbLine),
			{ 0,0,0 },
			20.F/* CORRIDOR */
		); 


		if (pos != m_player.getTile()) {

			/* Compute the move and update the player's tile */
			Server::Client::MOVE_LIST direction = computeMoveDirection(m_player.getTile(), pos);
			m_client->move(direction);
			m_player.setTile(pos);
			
			/* Get all tiles pheromone value */
			std::vector<float> p = m_client->getPheromones();
			std::for_each(p.begin(), p.end(), [](const float& f) {std::cout << "," << f; });
			std::cout << std::endl;
			m_pManager->setPheromones(m_client->getPheromones());
		}

		// Fake gravity stuff
		if (m_terrain.isInSamplableRegion(playerPos.x, playerPos.z) && !m_useDbgPlayer)
			playerPos.y = m_terrain.getHeight(playerPos.x, playerPos.z) + 1.5f;

		m_player.setPosition(playerPos);
		m_player.updateCamera();


		if (m_useDbgPlayer) {
			m_debugPlayer.step(delta);
			m_pManager->step(delta, m_debugPlayer.getCamera().getPosition());
		} else {
			m_player.step(delta);
			m_pManager->step(delta, m_player.getCamera().getPosition());
		}

		m_score.step(m_player.getPosition());
	}
		
	void onRender() override
	{
		const Renderer::Camera& camera = m_useDbgPlayer ? m_debugPlayer.getCamera() : m_player.getCamera();

		Renderer::Frustum cameraFrustum = Renderer::Frustum::createFrustumFromPerspectiveCamera(camera);

		m_pipeline.bind();
		Renderer::clear();

		m_sandTexture.bind(0);
		for (const auto& [position, chunk] : m_terrain.getChunks()) {
			const AABB& chunkAABB = chunk.getMesh().getBoundingBox();

			if (!cameraFrustum.isOnFrustum(chunkAABB))
				continue;

			Renderer::renderMesh(camera, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 }, chunk.getMesh());
		}

		Renderer::renderMesh(camera, { 0,0,0 }, { 1,1,1 }, m_mazeMesh);

		// TODO replace the maze mesh by actual walls and pillars meshes
		// DOTOO do instanced rendering on walls and pillars
		//for (unsigned int x = 0; x < m_maze.nbLine; x++) {
		//	for (unsigned int y = 0; y < m_maze.nbColumn; y++) {
		//		constexpr float W = MazeMeshGenerator::WALL_SIZE;
		//		constexpr float C = MazeMeshGenerator::CORRIDOR_SPACE;
		//		constexpr float H = MazeMeshGenerator::WALL_HEIGHT;
		//		Renderer::renderMesh(camera, { C,0,C/2.f }, { W,H,(C-W)/2 }, m_wallMesh);
		//	}
		//}

		m_props.render(camera);
		m_player.render(camera);
		m_props.render(camera);
		m_sky.render(camera);
		m_pManager->render(camera); 

		if (DebugWindow::renderAABB()) {
			for (unsigned int y = 0; y < 5; y++) {
				for (unsigned int x = 0; x < 5; x++) {
					AABB aabb = AABB::make_aabb(glm::vec3(x * 20, 0, y * 20), glm::vec3((x + 1) * 20, 5.f, (y + 1) * 20));
					Renderer::renderAABBDebugOutline(camera, aabb);
				}
			}
		}

		m_pipeline.unbind();
		m_pipeline.renderPipeline();
	}

	void onImGuiRender() override
	{
		ImGui::Text("This is the playground for tests ! ");

		ImGui::Checkbox("Use debug player", &m_useDbgPlayer);

		m_pipeline.onImGuiRender();

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ 600, 200 });
		if (ImGui::Begin("Score!", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration)) {
			ImGui::SetWindowFontScale(4);
			//ImGui::PushFont(nullptr); // TODO set the score's font
			ImGui::Text("SCORE: %u", m_score.getScore());
		}
		ImGui::End();
	}

};


