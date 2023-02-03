#pragma once

#include "libAntMaze.h"
#include "CException.h"
#include "CTile.h"
#include "CMazeGenerator.h"

#ifdef __cplusplus // If we use C++

#include <cstdint>

#define AllocationFailed 1
#define InvalidSize 2
#define InvalidNestPosition 3

struct Maze;


class CMaze
{
private:
	uint32_t nbLine;
	uint32_t nbColumn;
	uint32_t nestLine;
	uint32_t nestColumn;
	CTile** tiles;

public:

	/*
	Default contructor for a maze, create a 2x2 maze with no inside wall, the nest on the bottom left and a food source on the top right
	In : /
	Preconditions : /
	Out : /
	Postcondition : 
		- The maze has been successfully created OR
		- Error 1 : AllocationFailed
	*/
	CMaze();


	/*
	Create a maze from parameters
	In : 
		- nbLineIn, nbColumnIn : uint32_t, size of the maze, both must be >= 2
		- nestLineIn, nestColumnIn : uint32_t, position of the nest in the maze, must respectively be < to nbLineIn and nbColumnIn
		- nbFood : uint32_t, number of food sources in the maze
		- difficulty : uint32_t, difficulty of the maze
		- algoToUse : uint32_t, algorithm to use to generate the maze, liste avaiable in the CMazeGenerator.h file
	Preconditions : /
	Out : /
	Postcondition :
		- The maze has been successfully created OR
		- Error 1 : AllocationFailed  OR
		- Error 2 : InvalidSize, the size of the maze is invalid OR
		- Error 3 : InvalidNestPosition, the position of the nest is not in the maze
	*/
	CMaze(uint32_t nbLineIn, uint32_t nbColumnIn, uint32_t nestLineIn, uint32_t nestColumnIn, uint32_t nbFood, uint32_t difficulty, uint32_t algoToUse);


	/*
	Create an identic maze from another one
	In :
		- maze : CMaze, maze to copy
	Preconditions : /
	Out : /
	Postcondition : 
		- The maze has been successfully created OR
		- Error 1 : AllocationFailed
	*/
	CMaze(const CMaze& maze);


	/*
	Destructor of the CMaze class, free the tiles
	*/
	~CMaze();



	// GETTEURS ET SETTEURS

public:

	uint32_t getNbLine() {
		return nbLine;
	}

	uint32_t getNbColumn() {
		return nbColumn;
	}

	uint32_t getNestLine() {
		return nestLine;
	}

	uint32_t getNestColumn() {
		return nestColumn;
	}
	
	/*
	Return a pointer to a copy of the tile at the set coordinates
	*/
	CTile* getTile(uint32_t line, uint32_t column);


	void setTile(uint32_t line, uint32_t column, CTile* tile);


	// OTHER METHODS

public:

	/*
	Return the Maze Object into an instance of the Maze Struct
	In : /
	Preconditions : /
	Out :
		- Maze* : A pointer to a copy of the Maze Object in a Maze Struct
	Postcondition :
		- The maze has been successfully created and returned OR
		- Error 1 : AllocationFailed
	*/
	Maze* convertToMazeStruct();

	/*
	*/
	void printMaze();

	// OPERATORS

public:

	/*
	Create an identic maze from another one
	In :
		- maze : CMaze, maze to copy
	Preconditions : /
	Out : 
		- CMaze : copy of the initial maze
	Postcondition :
		- The maze has been successfully created and returned OR
		- Error 1 : AllocationFailed
	*/
	CMaze operator=(CMaze maze);

};


#else // If we use C

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct CMaze {
	uint32_t nbLine;
	uint32_t nbColumn;
	uint32_t nestLine;
	uint32_t nestColumn;
	CTile** tiles;
} CMaze;

#endif


// access functions
#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif


EXPORT_C CMaze* CMaze_new(void);

EXPORT_C CMaze* CMaze_new_with_param(uint32_t nbLineIn, uint32_t nbColumnIn, uint32_t nestLineIn, uint32_t nestColumnIn, uint32_t nbFood, uint32_t difficulty, uint32_t algoToUse);

EXPORT_C void CMaze_delete(CMaze* maze);

EXPORT_C uint32_t CMaze_getNbLine(CMaze* maze);

EXPORT_C uint32_t CMaze_getNbColumn(CMaze* maze);

EXPORT_C uint32_t CMaze_getNestLine(CMaze* maze);

EXPORT_C uint32_t CMaze_getNestColumn(CMaze* maze);

EXPORT_C CTile* CMaze_getTile(CMaze* maze, uint32_t line, uint32_t column);

EXPORT_C void CMaze_setTile(CMaze* maze, uint32_t line, uint32_t column, CTile* tile);

EXPORT_C Maze* CMaze_convertToMazeStruct(CMaze* maze);

EXPORT_C void CMaze_printMaze(CMaze* maze);