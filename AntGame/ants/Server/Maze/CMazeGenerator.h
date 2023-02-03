#pragma once

#include "CMaze.h"


#ifdef __cplusplus

#include <random>
#include <stack>

class CMaze;
class CTile;

class CMazeGenerator
{

public:

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
	static void generateEmptyMaze(CMaze* maze);

	static void generateRandomMaze(CMaze* maze);


	static uint32_t* generateFoodEmplacement(CMaze* maze, uint32_t nbFood);

	static uint32_t getRandomNeighbor(CMaze* maze, uint32_t tilePosition, bool* isTileMarqued);

	static void DFCarving(CMaze* maze, CTile* tiles, uint32_t foodLine, uint32_t foodColumn);


	static void generateMazeWithDepthFirstAlgo(CMaze* maze, uint32_t nbFood, uint32_t difficulty);
};


#else // If we use C

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct CMazeGenerator {

} CTile;

#endif


EXPORT_C CMazeGenerator* CMazeGenerator_new(void);

EXPORT_C void CMazeGenerator_delete(CMazeGenerator* mazeGenerator);

EXPORT_C void CMazeGenerator_generateEmptyMaze(CMazeGenerator* mazeGenerator, CMaze* maze);



EXPORT_C uint32_t* CMaze_generateFoodEmplacement(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood);

EXPORT_C uint32_t CMaze_getRandomNeighbor(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t tilePosition, bool* isTileMarqued);

EXPORT_C void CMaze_DFCarving(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tiles, uint32_t foodLine, uint32_t foodColumn);


EXPORT_C void CMazeGenerator_generateMazeWithDepthFirstAlgo(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood, uint32_t difficulty);
