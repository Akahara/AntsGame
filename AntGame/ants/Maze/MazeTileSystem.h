#pragma once

#include <glm/glm.hpp>

#include "marble/Utils/Mathf.h"

class MazeTileSystem
{
public:


	static glm::uvec2 getTileCoordinates(const glm::vec3& position, const glm::uvec2& mazeDimensions, const glm::vec3& startOfMaze, float tileSize) {


		float mazeLength = mazeDimensions.x * tileSize;
		float mazeWidth = mazeDimensions.y * tileSize;


		float x = float(position.x - startOfMaze.x) / mazeLength;
		unsigned int x_tile = (unsigned int)(x * (mazeDimensions.x - 1));


		float y = float(position.z - startOfMaze.z) / mazeWidth;

		
		unsigned int y_tile = (unsigned int)(y * (mazeDimensions.y - 1));

		return { x_tile, y_tile };

	}

};

