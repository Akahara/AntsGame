#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "marble/Sandbox/Scene.h"
#include "marble/world/Player.h"
#include "marble/world/Sky.h"

#include "../Maze/MazeMeshGenerator.h"
#include "../GameLogic/AntsPlayer.h"

class Playground : public Scene {
private:
	World::Sky m_sky{World::Sky::SkyboxesType::SAND};

	float m_realTime = 0;

	bool       m_useDbgPlayer = false;
	AntsPlayer m_player;
	Player     m_debugPlayer;

	Maze m_maze;

	Renderer::Mesh    m_mazeMesh;
	Renderer::Texture m_sandTexture = Renderer::Texture("res/textures/sand1.jpg");

	glm::vec2 m_tpssettings = { 4,3 };

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
	}

	void step(float delta) override
	{
		m_realTime += delta;

		Player &activePlayer = m_useDbgPlayer ? m_debugPlayer : m_player;
		activePlayer.step(delta);
	}

	void onRender() override
	{
		Player &activePlayer = m_useDbgPlayer ? m_debugPlayer : m_player;
		Renderer::Camera& camera = activePlayer.getCamera();

		Renderer::clear();
		glDisable(GL_CULL_FACE);
		m_sandTexture.bind(0);
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

	CAMERA_IS_PLAYER(m_player)
};


