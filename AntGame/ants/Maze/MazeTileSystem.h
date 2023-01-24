#pragma once
#include "marble/Utils/Mathf.h"
#include <glm/glm.hpp>
class MazeTileSystem
{
public:


	static glm::uvec2 getTileCoordinates(const glm::vec3& position, const glm::uvec2& mazeDimensions, const glm::vec3& startOfMaze, float tileSize) {


		float mazeLength = mazeDimensions.x * tileSize;
		float mazeWidth = mazeDimensions.y * tileSize;


		float x = float(position.x - startOfMaze.x) / mazeLength;
		unsigned int x_tile = x * (mazeDimensions.x - 1);


		float y = float(position.z - startOfMaze.z) / mazeWidth;

		
		unsigned int y_tile = y * (mazeDimensions.y - 1);

		return { x_tile, y_tile };

	}

};

