#pragma once

#include <glm/glm.hpp>

#include "marble/Utils/Mathf.h"
#include "MazeGeneration_proj/libAntMaze.h"


class MazeTileSystem
{
public:


	static glm::uvec2 getTileCoordinates(const glm::vec3& position, const glm::uvec2& mazeDimensions, const glm::vec3& startOfMaze, float tileSize);
	static bool hasFoodOnTile(const Maze& maze, const glm::uvec2& tile);

};

