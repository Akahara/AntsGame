#pragma once

#include <glm/vec2.hpp>
#include <array>

#include "marble/abstraction/Mesh.h"

#include "MazeGeneration_proj/libAntMaze.h"

namespace MazeMeshGenerator
{
constexpr unsigned char NORTH    = 1;
constexpr unsigned char EAST     = 2;
constexpr unsigned char SOUTH    = 4;
constexpr unsigned char WEST     = 8;
constexpr unsigned char HAS_FOOD = 16;
constexpr unsigned char HAS_NEST = 32;

constexpr float WALL_SIZE = 5.f;
constexpr float WALL_HEIGHT = 20.F;
constexpr float CORRIDOR_SPACE = 20.F;

Renderer::Mesh generateMazeMesh(const Maze &maze);

};

