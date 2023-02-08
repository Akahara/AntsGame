#include "MazeTileSystem.h"


glm::uvec2 MazeTileSystem::getTileCoordinates(const glm::vec3& position, const glm::uvec2& mazeDimensions, const glm::vec3& startOfMaze, float tileSize) {


	float mazeLength = mazeDimensions.x * tileSize;
	float mazeWidth = mazeDimensions.y * tileSize;


	float x = float(position.x - startOfMaze.x) / mazeLength;
	unsigned int x_tile = (unsigned int)(x * (mazeDimensions.x));


	float y = float(position.z - startOfMaze.z) / mazeWidth;


	unsigned int y_tile = (unsigned int)(y * (mazeDimensions.y));

	return { x_tile, y_tile };

}


bool MazeTileSystem::hasFoodOnTile(const Maze& maze, const glm::uvec2& tile) {

	return ((maze.tiles[tile.y * maze.nbColumn + tile.x] & 16));

}