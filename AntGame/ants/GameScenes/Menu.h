#pragma once

#include "src/Sandbox/Scene.h"
#include "../GameScenes/InGame.h"


class Menu : public Scene {

private:

public:


	void step(float delta) override {

	}

	void onRender() override {

	}

	void onImGuiRender() override {

		ImGui::Begin("Play the damn game");

		if (ImGui::Button("Play")) {

			SceneManager::registerScene<>


		}



		ImGui::End();

	}


};