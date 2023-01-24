#pragma once

#include "marble/Sandbox/Scene.h"
#include "marble/world/Player.h"
#include "marble/world/Sky.h"

#include "../Maze/MazeMeshGenerator.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Playground : public Scene {


private:

	World::Sky m_sky{World::Sky::SkyboxesType::SAND};


	float m_realTime = 0;


	Player m_player;


	Renderer::Mesh m_mesh = MazeMeshGenerator::generateMazeMesh(20,20);


public:

	Playground() {


		


	}


	void step(float delta) override {

		m_realTime += delta;
		m_player.step(delta);

	}

	void onRender() override {

		Renderer::clear();
		glDisable(GL_CULL_FACE);
		Renderer::renderMesh(getCamera(), { 0,0,0 }, { 1,1,1 }, m_mesh);
		m_sky.render(getCamera());


	}


	void onImGuiRender() override {

		ImGui::Text("This is the playground for tests ! ");

	}

	CAMERA_IS_PLAYER(m_player)
};


