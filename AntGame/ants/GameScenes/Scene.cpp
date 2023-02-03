#include "Scene.h"

#include <iostream>

namespace SceneManager {

static std::unique_ptr<Scene> s_activeScene = nullptr;
static std::unique_ptr<Scene> s_nextScene = nullptr;

void step(float delta)
{
  if (s_nextScene != nullptr)
    s_activeScene = std::move(s_nextScene);
  s_activeScene->step(delta);
}

void onRender()
{
  s_activeScene->onRender();
}

void onImGuiRender()
{
  s_activeScene->onImGuiRender();
}

void switchToScene(std::unique_ptr<Scene> &&scene)
{
  s_nextScene = std::move(scene);
  std::cout << "switching scene" << std::endl;
}

void shutdown()
{
  s_activeScene = nullptr;
}

}