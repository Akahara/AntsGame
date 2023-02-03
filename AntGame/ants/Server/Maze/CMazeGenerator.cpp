#include "CMazeGenerator.h"


/*
Create a maze with only borders, a nest on the bottom right corner and a fodd source on the top left corner
In :
	- maze : CMaze*, the maze that we work on
Preconditions :
	- The nbLine, nbColumn and tiles atributes of the maze must be initialized and coherent (enouth memory for the tiles)
Out : /
Postcondition :
	- The tiles of the maze have been created in the maze
*/
void CMazeGenerator::generateEmptyMaze(CMaze* maze) {
	uint32_t nbLine = maze->getNbLine();
	uint32_t nbColumn = maze->getNbColumn();
	for (uint32_t line = 0; line < nbLine; line++) {
		for (uint32_t column = 0; column < nbColumn; column++) {
			if (line == 0 && column == 0) { // Top Left corner
				maze->setTile(line, column, new CTile(true, false, true, false, true, false));
			}
			else if (line == 0 && column == nbColumn - 1) { // Top Right corner
				maze->setTile(line, column, new CTile(true, false, false, true, false, false));
			}
			else if (line == nbLine - 1 && column == 0) { // Bottom Left corner
				maze->setTile(line, column, new CTile(false, true, true, false, false, false));
			}
			else if (line == nbLine - 1 && column == nbColumn - 1) { // Bottom Right corner
				maze->setTile(line, column, new CTile(false, true, false, true, false, true));
			}
			else if (line == 0) { // Top border
				maze->setTile(line, column, new CTile(true, false, false, false, false, false));
			}
			else if (line == nbLine - 1) { // Bottom border
				maze->setTile(line, column, new CTile(false, true, false, false, false, false));
			}
			else if (column == 0) { // Left border
				maze->setTile(line, column, new CTile(false, false, true, false, false, false));
			}
			else if (column == nbColumn - 1) { // Right border
				maze->setTile(line, column, new CTile(false, false, false, true, false, false));
			}
			else {
				maze->setTile(line, column, new CTile(false, false, false, false, false, false));
			}
		}
	}
}

/*
Do nothing
*/
void CMazeGenerator::generateRandomMaze(CMaze* maze)
{
	//srand(time(NULL));   rand() % 2;
	int nbLine = maze->getNbLine();
	int nbColumn = maze->getNbColumn();
	int nestLine = maze->getNestLine();
	int nestColumn = maze->getNestColumn();
}



uint32_t* CMazeGenerator::generateFoodEmplacement(CMaze* maze, uint32_t nbFood) {
	uint32_t nestLine = maze->getNestLine();
	uint32_t nestColumn = maze->getNestColumn();
	uint32_t minRangeFoodNestLine = (maze->getNbLine() / 2) - 1;
	uint32_t minRangeFoodNestColumn = (maze->getNbColumn() / 2) - 1;
	uint32_t* foodCoordinates = (uint32_t*)malloc(sizeof(uint32_t) * nbFood * 2); // coordinates of food : [0] -> lineFood0, [1] -> columnFood0, [2] -> lineFood1 ...
	srand(time(NULL));
	uint32_t foodLine, foodColumn, numFood;
	for (numFood = 0; numFood < nbFood; numFood++) {
		foodLine = nestLine;
		foodColumn = nestColumn;
		while ((foodLine < nestLine + minRangeFoodNestLine and foodLine > nestLine - minRangeFoodNestLine)
			or (foodColumn < nestColumn + minRangeFoodNestColumn and foodColumn > nestColumn - minRangeFoodNestColumn))
		{
			foodLine = rand() % maze->getNbLine();
			foodColumn = rand() % maze->getNbColumn();
			for (uint32_t numFood2 = 0; numFood2 < numFood; numFood2++) { // If slot is already a food, reset new food position
				if (foodLine == foodCoordinates[2 * numFood2] and foodColumn == foodCoordinates[2 * numFood2 + 1]) {
					foodLine = nestLine;
					foodColumn = nestColumn;
				}
			}
		}
		foodCoordinates[2 * numFood] = foodLine;
		foodCoordinates[2 * numFood + 1] = foodColumn;
	}
	return foodCoordinates;
}

/*
Return the position of the neighbor in the tiles list
*/
uint32_t CMazeGenerator::getRandomNeighbor(CMaze* maze, uint32_t tilePosition, bool* isTileMarqued) {
	uint32_t nbLine = maze->getNbLine();
	uint32_t nbColumn = maze->getNbColumn();
	uint32_t tileColumn = tilePosition % nbColumn;
	uint32_t tileLine = (tilePosition - tileColumn) / nbLine;
	uint32_t neighborPosition = tilePosition;

	// Generation of the directions to check for the neigbors
	//srand(time(NULL));
	uint32_t* directionList = (uint32_t*)malloc(sizeof(uint32_t) * 4);
	uint32_t nbDirectionInList = 0;
	uint32_t direction, dir;
	bool inList;
	while (nbDirectionInList < 4) {
		inList = false;
		direction = rand() % 4; // 0 : N, 1 : S, 2 : W, 3 : E
		for (dir = 0; dir < nbDirectionInList && !inList; dir++) {
			if (direction == directionList[dir]) {
				inList = true;
			}
		}
		if (!inList) {
			directionList[dir] = direction;
			nbDirectionInList++;
		}
	}
	uint32_t neighborDirection;

	for (uint32_t i = 0; i < 4 && neighborPosition == tilePosition; i++) {
		neighborDirection = directionList[i];
		if (!((neighborDirection == 0 && tileLine == 0) || (neighborDirection == 1 && tileLine >= nbLine - 1) 
			|| (neighborDirection == 2 && tileColumn == 0) || (neighborDirection == 3 && tileColumn >= nbColumn - 1)))
		{
			switch (neighborDirection) {
			case 0:
				if (!isTileMarqued[tilePosition - nbColumn]) {
					neighborPosition = tilePosition - nbColumn;
				}
				break;
			case 1:
				if (!isTileMarqued[tilePosition + nbColumn]) {
					neighborPosition = tilePosition + nbColumn;
				}
				break;
			case 2:
				if (!isTileMarqued[tilePosition - 1]) {
					neighborPosition = tilePosition - 1;
				}
				break;
			case 3:
				if (!isTileMarqued[tilePosition + 1]) {
					neighborPosition = tilePosition + 1;
				}
				break;
			default:
				break;
			}
		}
	}
	free(directionList);
	return neighborPosition;
}


void CMazeGenerator::DFCarving(CMaze* maze, CTile* tiles, uint32_t foodLine, uint32_t foodColumn) {
	uint32_t nbLine = maze->getNbLine();
	uint32_t nbColumn = maze->getNbColumn();
	uint32_t tilePosition, neighborTile, tileLine, tileColumn;
	
	bool* isTileMarqued = (bool*) malloc(sizeof(bool) * nbLine * nbColumn);
	std::stack<uint32_t> tilesQueue;
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		isTileMarqued[i] = false;
	}
	isTileMarqued[nbColumn * foodLine + foodColumn] = true;
	tilesQueue.push(nbColumn * foodLine + foodColumn);
	while (!tilesQueue.empty()) {
		tilePosition = tilesQueue.top();
		neighborTile = CMazeGenerator::getRandomNeighbor(maze, tilePosition, isTileMarqued);
		if (neighborTile == tilePosition) { // Remove the actual tile, no unmarqued neighbors
			tilesQueue.pop();
		}
		else { // Break the wall between the two tiles, put the new tile in the queue and mark it
			if (neighborTile == tilePosition - nbColumn) { // Break the N wall of tile and S wall of neighbor
				tiles[tilePosition].setWallN(false);
				tiles[neighborTile].setWallS(false);
			}
			else if (neighborTile == tilePosition + nbColumn) { // Break the S wall of tile and N wall of neighbor
				tiles[tilePosition].setWallS(false);
				tiles[neighborTile].setWallN(false);
			}
			else if (neighborTile == tilePosition + 1) { // Break the E wall of tile and W wall of neighbor
				tiles[tilePosition].setWallE(false);
				tiles[neighborTile].setWallW(false);
			}
			else if (neighborTile == tilePosition - 1) { // Break the W wall of tile and E wall of neighbor
				tiles[tilePosition].setWallW(false);
				tiles[neighborTile].setWallE(false);
			}
			tilesQueue.push(neighborTile);
			isTileMarqued[neighborTile] = true;
		}
	}
	free(isTileMarqued);
}




void CMazeGenerator::generateMazeWithDepthFirstAlgo(CMaze* maze, uint32_t nbFood, uint32_t difficulty) {
	uint32_t nbLine = maze->getNbLine();
	uint32_t nbColumn = maze->getNbColumn();
	uint32_t nestLine = maze->getNestLine();
	uint32_t nestColumn = maze->getNestColumn();
	uint32_t line, column;

	// Initialization of the new tiles
	CTile* tiles = (CTile*) malloc(sizeof(CTile) * nbLine * nbColumn);
	for (line = 0; line < nbLine; line++) {
		for (column = 0; column < nbColumn; column++) {
			tiles[nbColumn * line + column] = CTile(true, true, true, true, false, false);
		}
	}
	tiles[nbColumn * nestLine + nestColumn].setNest(true);


	// Gestion of the food in the new tiles
	uint32_t* foodCoordinates = CMazeGenerator::generateFoodEmplacement(maze, nbFood);
	for (uint32_t numFood = 0; numFood < nbFood; numFood++) {
		tiles[nbColumn * foodCoordinates[2 * numFood] + foodCoordinates[2 * numFood + 1]].setFood(true);
	}


	// Carving the walls of the new tiles
	srand(time(NULL));
	CMazeGenerator::DFCarving(maze, tiles, foodCoordinates[0], foodCoordinates[1]);
	//CMazeGenerator::DFCarving(maze, tiles, foodCoordinates[0], foodCoordinates[1]);

	
	// Edition of the maze to put the new tiles
	for (line = 0; line < nbLine; line++) {
		for (column = 0; column < nbColumn; column++) {
			maze->setTile(line, column, new CTile(tiles[nbColumn * line + column]));
		}
	}

	free(foodCoordinates);
}





EXPORT_C CMazeGenerator* CMazeGenerator_new(void) {
	return new CMazeGenerator();
}

EXPORT_C void CMazeGenerator_delete(CMazeGenerator* mazeGenerator) {
	delete mazeGenerator;
}

EXPORT_C void CMazeGenerator_generateEmptyMaze(CMazeGenerator* mazeGenerator, CMaze* maze) {
	return mazeGenerator->generateEmptyMaze(maze);
}


EXPORT_C uint32_t* CMaze_generateFoodEmplacement(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood) {
	return mazeGenerator->generateFoodEmplacement(maze, nbFood);
}

EXPORT_C uint32_t CMaze_getRandomNeighbor(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t tilePosition, bool* isTileMarqued) {
	return mazeGenerator->getRandomNeighbor(maze, tilePosition, isTileMarqued);
}

EXPORT_C void CMaze_DFCarving(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tiles, uint32_t foodLine, uint32_t foodColumn) {
	mazeGenerator->DFCarving(maze, tiles, foodLine, foodColumn);
}


EXPORT_C void CMazeGenerator_generateMazeWithDepthFirstAlgo(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood, uint32_t difficulty) {
	return mazeGenerator->generateMazeWithDepthFirstAlgo(maze, nbFood, difficulty);
}
