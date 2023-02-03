#include "Server/Main.h"
#include <iostream>
/*
#include "marble/vendor/imgui/imgui.h"
#include "marble/vendor/imgui/imgui_impl_glfw.h"
#include "marble/vendor/imgui/imgui_impl_opengl3.h"

#include "marble/abstraction/Window.h"
#include "marble/abstraction/Inputs.h"

#include "marble/Sandbox/Scene.h"
#include "marble/Utils/Debug.h"

#include "marble/Sandbox/Tests.h"

#include "GameScenes/Playground.h"



inline long long nanoTime()
{
  using namespace std::chrono;
  return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

*/
int main()
{
    /*
  Window::createWindow(16 * 70, 9 * 70, "test");

  Window::setVisible(true);
  Window::setPosition(400, 100);
  Window::capFramerate();
  Inputs::observeInputs();

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(Window::getWindowHandle()), true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  Renderer::SkyRenderer::init();
  Renderer::init();
  SceneManager::init();


  SceneManager::registerScene<Playground>("Playground");




  SceneManager::switchToScene(1);

  //===========================================================//

  unsigned int frames = 0;
  auto firstTime = nanoTime();
  auto lastSec = firstTime;

  while (!Window::shouldClose()) {

    auto nextTime = nanoTime();
    auto delta = nextTime - firstTime;
    float realDelta = delta / 1E9f;
    firstTime = nextTime;

    Window::pollUserEvents();
    Inputs::updateInputs();
    frames++;

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    SceneManager::step(realDelta);
    SceneManager::onRender();
    SceneManager::onImGuiRender();
    DebugWindow::onImGuiRender();

    Renderer::Shader::unbind(); // unbind shaders before ImGui's new frame, so it won't try to restore a shader that has been deleted

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    Window::sendFrame();

    if (lastSec + 1E9 < nextTime) {
      char title[50];
      sprintf_s(title, 50, "Some game, %dfps", frames);
      Window::renameWindow(title);
      lastSec += (long long)1E9;
      frames = 0;
    }
  }

  SceneManager::shutdown();

  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
    */

    std::string adress = "192.168.1.23";
    unsigned short port = 88000;
    boost::asio::io_context io_context1;

    Client client1{ io_context1, adress, port };

    constexpr unsigned char NORTH = 1;
    constexpr unsigned char EAST = 2;
    constexpr unsigned char SOUTH = 4;
    constexpr unsigned char WEST = 8;
    constexpr unsigned char HAS_FOOD = 16;
    constexpr unsigned char HAS_NEST = 32;

    client1.join(1);

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));  


  return 0;
}
