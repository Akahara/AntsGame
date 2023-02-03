#pragma once

#include "marble/abstraction/Window.h"
#include "marble/abstraction/UnifiedRenderer.h"

#include "Scene.h"
#include "Playground.h"

class Menu : public Scene {
private:
    Renderer::Texture m_backgroundTexture;

public:
	Menu()
	{
		m_backgroundTexture = Renderer::Texture("res/textures/no_texture.png"); // TODO: make an appropriate background texture
		Window::captureMouse(false);
	}

	void step(float delta) override
	{

	}

	void onRender() override
	{
		Renderer::clear();
		Renderer::renderDebugGUIQuadWithTexture(m_backgroundTexture, { -1,-1 }, { 2,2 });
	}

	void onImGuiRender() override
	{
		glm::uvec2 winSize{ Window::getWinWidth(), Window::getWinHeight() };
		glm::uvec2 menuSize = winSize / 5u;
		ImGui::SetNextWindowPos({ (winSize.x-menuSize.x)/2.f, (winSize.y-menuSize.y)/2.f });
		ImGui::SetNextWindowSize({ (float)menuSize.x, (float)menuSize.y });
		ImGui::Begin("Play the damn game", nullptr, ImGuiWindowFlags_NoDecoration);

		if (ImGui::Button("Play")) {
		  Window::captureMouse(true);
		  SceneManager::switchToScene(std::make_unique<Playground>());
		}

		ImGui::End();
	}
};