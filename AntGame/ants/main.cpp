#include <chrono>
#include <iostream>
#include <memory>

#include "marble/vendor/imgui/imgui.h"
#include "marble/vendor/imgui/imgui_impl_glfw.h"
#include "marble/vendor/imgui/imgui_impl_opengl3.h"

#include "marble/abstraction/Window.h"
#include "marble/abstraction/Inputs.h"
#include "marble/Utils/Debug.h"

#include "GameScenes/Scene.h"
#include "GameScenes/Playground.h"
#include "GameScenes/Menu.h"

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/signal.h>
#define sprintf_s snprintf
#endif

inline long long nanoTime()
{
  using namespace std::chrono;
  return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int main()
{
  #if defined(WIN32) || defined(_WIN32)
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 2), &wsa);
  #else
  /* Ignore broken pipe signals. */
  signal(SIGPIPE, SIG_IGN);
  #endif

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

  SceneManager::switchToScene(std::make_unique<Menu>());

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
    
  /*
    std::string adress = "192.168.1.23";
    unsigned short port = 88000;
    boost::asio::io_context io_context1;

    Client client1{ io_context1, adress, port };

    client1.join(1);

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));  
  */

  #if defined(WIN32) || defined(_WIN32)
  WSACleanup();
  #endif

  return 0;
}
