#pragma once

#include <boost/asio.hpp>

#include "marble/abstraction/Window.h"
#include "marble/abstraction/UnifiedRenderer.h"
#include "Scene.h"
#include "Playground.h"

class Menu : public Scene {
private:
    Renderer::Texture m_backgroundTexture;
	char        m_hostIp[32]     = "localhost";
	std::string m_localIpAddress = "pending...";

public:
	Menu()
	{
		m_backgroundTexture = Renderer::Texture("res/textures/no_texture.png"); // TODO: make an appropriate background texture
		Window::captureMouse(false);

		try {
			// https://stackoverflow.com/questions/2674314/get-local-ip-address-using-boost-asio
			// the easiest way to get the local ip address it to ping a server
			// and read the destination address of the answer
			using boost::asio::ip::udp;
			boost::asio::io_service netService;
			udp::resolver resolver(netService);
			udp::resolver::query query(udp::v4(), "google.com", "");
			udp::resolver::iterator endpoints = resolver.resolve(query);
			udp::endpoint ep = *endpoints;
			udp::socket socket(netService);
			socket.connect(ep);
			boost::asio::ip::address addr = socket.local_endpoint().address();
			m_localIpAddress = addr.to_string();
		} catch (std::exception &e) {
			std::cerr << "Could not fetch local ip address: " << e.what() << std::endl;
		}
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
		ImGui::SetNextWindowPos({ winSize.x/2.f, winSize.y/2.f }, ImGuiCond_Always, { .5f, .5f });
		ImGui::Begin("Play the damn game", nullptr, ImGuiWindowFlags_NoDecoration);

		ImGui::InputText("Host address", m_hostIp, sizeof(m_hostIp));
		ImGui::BeginDisabled();
		ImGui::InputText("Your address", (char*)m_localIpAddress.c_str(), m_localIpAddress.size());
		ImGui::EndDisabled();

		if (ImGui::Button("Play")) {
		  Window::captureMouse(true);
		  SceneManager::switchToScene(std::make_unique<Playground>());
		}

		ImGui::SetWindowSize({ 0,0 }, ImGuiCond_Always);
		ImGui::End();
	}
};