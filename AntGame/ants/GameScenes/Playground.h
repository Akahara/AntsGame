#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "marble/Sandbox/Scene.h"
#include "marble/world/Player.h"
#include "marble/world/Sky.h"

#include "../Maze/MazeMeshGenerator.h"
#include "../GameLogic/AntsPlayer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Server/ServerInfos.h"
#include "../Maze/MazeTileSystem.h"

class Playground : public Scene {
private:
	World::Sky m_sky{World::Sky::SkyboxesType::SAND};

	float m_realTime = 0;

	bool       m_useDbgPlayer = false;
	AntsPlayer m_player;
	Player     m_debugPlayer;

	Terrain::Terrain  m_terrain;
	Maze m_maze;

	Renderer::Mesh    m_mazeMesh;
	Renderer::Texture m_sandTexture = Renderer::Texture("res/textures/sand1.jpg");

	glm::vec2 m_tpssettings = { 5,1.4 };



public:

	Playground() 
	{
		constexpr unsigned int mazeSize = 20;
		ParamMaze params{
			mazeSize,
			mazeSize,
			0,
			0,
			1,
			2
		};

		m_maze = *generateMazeCPP(&params); // this interface is... bad to say the least, if you go to the body of generateMazeCPP you will find
											// an O(n) memory leak, what a performance!
		m_mazeMesh = MazeMeshGenerator::generateMazeMesh(m_maze);
		m_player.setMaze(m_maze);

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


		generateTerrain();		


	}

	void generateTerrain()
	{
		// 20x20 * 20x20 ~= 512x512 which is a good size of the erosion algorithm
		constexpr unsigned int chunkSize = 20;
		constexpr unsigned int chunkCount = 20;

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
			//Noise::ErosionSettings erosionSettings{};
			//Noise::erode(noiseMap, noiseMapSize, erosionSettings);
			Noise::rescaleNoiseMap(noiseMap, noiseMapSize, noiseMapSize, 0, 1, 0, /*terrain height*/9.f);
			Noise::outlineNoiseMap(noiseMap, noiseMapSize, noiseMapSize, -5, 2);
			Terrain::HeightMap* heightMap = new Terrain::ConcreteHeightMap(noiseMapSize, noiseMapSize, noiseMap);
			m_terrain = Terrain::generateTerrain(heightMap, chunkCount, chunkCount, chunkSize);
		}
	}


	void step(float delta) override {

		m_realTime += delta;

		glm::vec3 playerPos = m_player.getPosition();
		glm::uvec2 pos = MazeTileSystem::getTileCoordinates(playerPos, { 20,20 }, { 0,0,0 }, 25.f /* CORRIDOR+WALSIZE */ );
		
		if (pos != m_player.getTile()) {
			m_player.setTile(pos);
			Server::sendInfos(pos); // mock
		}

		
		if (m_terrain.isInSamplableRegion(playerPos.x, playerPos.z))
			playerPos.y = m_terrain.getHeight(playerPos.x, playerPos.z) +1.5;
		

		m_player.setPosition(playerPos);
		m_player.updateCamera();

		if (m_useDbgPlayer) {
			m_debugPlayer.step(delta);
		}
		else {
			m_player.step(delta);
		}
	}

	void onRender() override
	{
		const Renderer::Camera& camera = m_useDbgPlayer ? m_debugPlayer.getCamera() : m_player.getCamera();

		Renderer::Frustum cameraFrustum = Renderer::Frustum::createFrustumFromPerspectiveCamera(camera);

		Renderer::clear();

		m_sandTexture.bind(0);
		for (const auto& [position, chunk] : m_terrain.getChunks()) {
			const AABB& chunkAABB = chunk.getMesh().getBoundingBox();

			if (!cameraFrustum.isOnFrustum(chunkAABB))
				continue;

			Renderer::renderMesh(camera, glm::vec3{ 0, 1, 0 }, glm::vec3{ 1, 1, 1 }, chunk.getMesh());
		}
		Renderer::renderMesh(camera, { 0,0,0 }, { 1,1,1 }, m_mazeMesh);
		m_player.render(camera);
		m_sky.render(camera);

	}

	void onImGuiRender() override
	{
		ImGui::Text("This is the playground for tests ! ");

		if (ImGui::DragFloat2("TPS Settings", &m_tpssettings.x, 0.2f, 0.5f, 5)) {
			m_player.setTPSDistances(m_tpssettings);
		}

		ImGui::Checkbox("Use debug player", &m_useDbgPlayer);
	}

	CAMERA_NOT_DEFINED();
};


