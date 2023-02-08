#pragma once

#include <string>
#include <functional>
#include <memory>

#include "marble/vendor/imgui/imgui.h"

class Scene {
public:
  virtual ~Scene() = default;

  virtual void step(float delta) = 0; // equivalent to OnUpdate in game engines (eg. Unity)
  virtual void onRender() = 0;
  virtual void onImGuiRender() = 0;
};

namespace SceneManager {

void step(float delta);
void onRender();
void onImGuiRender();

void switchToScene(std::unique_ptr<Scene> &&scene);
void shutdown();

}