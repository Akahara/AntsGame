#pragma once

#include "marble/Sandbox/Scene.h"
#include "marble/world/Player.h"
#include "marble/world/Sky.h"
#include "../AntGame/ants/GameLogic/AntsPlayer.h"

#include "../Maze/MazeMeshGenerator.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Playground : public Scene {


private:

	World::Sky m_sky{World::Sky::SkyboxesType::SAND};


	float m_realTime = 0;


	AntsPlayer m_player;


	Renderer::Mesh m_mesh = MazeMeshGenerator::generateMazeMesh(20,20);

	Renderer::Texture    m_sandTexture = Renderer::Texture("res/textures/sand1.jpg");

	bool m_lookAtCube = false;

	glm::vec3 m_tpssettings = { 4,3,4 };


public:

	Playground() {

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


	void step(float delta) override {

		m_realTime += delta;
		m_player.step(delta);

		if (m_lookAtCube) {
			m_player.getCamera().lookAt({ -10,-10,-10 });
			m_player.updateCamera();
		}

	}

	void onRender() override {


		Renderer::Camera& camera = m_player.getCamera();

		Renderer::clear();
		glDisable(GL_CULL_FACE);
		m_sandTexture.bind(0);
		Renderer::renderMesh(getCamera(), { 0,0,0 }, { 1,1,1 }, m_mesh);
		Renderer::renderDebugCube(getCamera(), { -10,-10,-10 }, { 3,3,3 });
		m_player.render();
		m_sky.render(getCamera());


	}


	void onImGuiRender() override {

		ImGui::Text("This is the playground for tests ! ");
		ImGui::Checkbox("Look at the cube", &m_lookAtCube);

		if (ImGui::DragFloat3("TPS Settings", &m_tpssettings.x, 0.2, 0, 5)) {
			m_player.setTPSDistances(m_tpssettings);
		}

		if (ImGui::Button("switch viewmode")) m_player.switchViewMode();

	}

	CAMERA_IS_PLAYER(m_player)
};


