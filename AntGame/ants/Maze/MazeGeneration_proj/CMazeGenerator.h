#pragma once

#include "CMaze.h"

#define AllocationFailed 1
#define InvalidPosition 3

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


	/*
	Generate coordinates to place the food in the maze ith a minimal range between the nest and the foods
	In :
		- maze : CMaze*, the maze that we work on
		- nbFood : uint32_t, the number of food to place in the maze
	Preconditions :
		- nbFood must be <= to (nbLine * nbColumn) - (((nbLine / 2) - 1) * ((nbColumn / 2) - 1) * 2)
	Out :
		- uint32_t* : the coordinates of the foods, in the format lineFood1, columnFood1, lineFood2, columnFood2, ...
	Postcondition :
		- The food coordinates has been intialisated and returned OR
		- Error 1 : AllocationFailed
	*/
	static uint32_t* generateFoodEmplacement(CMaze* maze, uint32_t nbFood);


	/*
	Return the position of a random unmarqued neighbor of the passed tilePosition in the tiles list (or tilePosition if no unmarqued neighbor)
	In :
		- maze : CMaze*, the maze that we work on
		- tilePosition : uint32_t, position of the tile in the maze
		- isTileMarqued : bool*, list that indicate if a tile has been marqued or not
	Preconditions :
		- tileposition must be in the maze
		- isTileMarqued's size must be >= to the size of the maze
	Out :
		- uint32_t : the position of the unmarqued neighbor OR the tile position if all its neighbors are already marqued
	Postcondition :
		- The position is returned OR
		- Error 1 : AllocationFailed
	*/
	static uint32_t getRandomNeighbor(CMaze* maze, uint32_t tilePosition, bool* isTileMarqued);


	/*
	Break the wall between tilePosition and neighborTile
	In :
		- maze : CMaze*, the maze that we work on
		- tiles : CTile*, the tiles we are woring on
		- tilePosition : uint32_t, position of the tile to break the wall
		- neighborTile : uint32_t, position of one of the neighbor of the tile
	Preconditions :
		- tilePosition and neighborTile must be in the maze
	Out : /
	Postcondition :
		- The wall is broken if the tiles are next to each other
	*/
	static void breakWall(CMaze* maze, CTile* tiles, uint32_t tilePosition, uint32_t neighborTile);


	/*
	Apply the Depth-First Algorithm to carve the walls of the maze
	In :
		- maze : CMaze*, the maze that we work on
		- tiles : CTile*, the tiles we are woring on
		- startLine : uint32_t, line of the tile here the alorithm will start, must be < to maze->getNbLine();
		- startColumn : uint32_t, column of the tile here the alorithm will start, must be < to maze->getNbColumn();
	Preconditions : /
	Out : /
	Postcondition :
		- The tiles has been carved OR
		- Error 1 : AllocationFailed OR
		- Error 3 : Invalid position, the starting position is out of the maze
	*/
	static void DFCarving(CMaze* maze, CTile* tiles, uint32_t startLine, uint32_t startColumn);


	/*
	Return the position of one of the neighbor of the tile with a wall in common
	In :
		- maze : CMaze*, the maze that we work on
		- tiles : CTile*, the tiles we are woring on
		- tilePosition : uint32_t, position of the tile in the maze
	Preconditions : /
	Out :
		- uint32_t : the position of the neighbor in the maze OR tilePosition if the tile has no all inside the maze beside the borders
	Postcondition :
		- The position of the maze is returned OR
		- Error 1 : AllocationFailed OR
		- Error 3 : InvalidPosition, the starting position is out of the maze
	*/
	static uint32_t getRandomNeighborDeadEnd(CMaze* maze, CTile* tile, uint32_t tilePosition);


	/*
	Remove the dead ends in a maze
	In :
		- maze : CMaze*, the maze that we work on
		- tiles : CTile*, the tiles we are woring on
	Preconditions : /
	Out : /
	Postcondition :
		- The dead ends has been removed OR
		- Error 1 : AllocationFailed
	*/
	static void removeDeadEnds(CMaze* maze, CTile* tiles);


	/*
	Generate the tiles of the maze with the Depth-First algorithm
	In :
		- maze : CMaze*, the maze that we work on
		- nbFood : uint32_t, number of food to put in the maze
		- difficulty : uint32_t, difficulty of the maze (1 : not a lot of walls; 2 : a lot of walls but no dead end; 3 : a lot of alls and dead ends
	Preconditions : /
	Out : /
	Postcondition :
		- The tiles of the maze has been generated with the Depth-First algorithm
		- Error 1 : AllocationFailed OR
		- Error 3 : Invalid position, the starting position for the carving is out of the maze
	*/
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



EXPORT_C uint32_t* CMazeGenerator_generateFoodEmplacement(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood);

EXPORT_C uint32_t CMazeGenerator_getRandomNeighbor(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t tilePosition, bool* isTileMarqued);

EXPORT_C void CMazeGenerator_breakWall(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tiles, uint32_t tilePosition, uint32_t neighborTile);

EXPORT_C void CMazeGenerator_DFCarving(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tiles, uint32_t startLine, uint32_t startColumn);

EXPORT_C uint32_t CMazeGenerator_getRandomNeighborDeadEnd(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tile, uint32_t tilePosition);

EXPORT_C void CMazeGenerator_removeDeadEnds(CMazeGenerator* mazeGenerator, CMaze* maze, CTile* tiles);

EXPORT_C void CMazeGenerator_generateMazeWithDepthFirstAlgo(CMazeGenerator* mazeGenerator, CMaze* maze, uint32_t nbFood, uint32_t difficulty);
