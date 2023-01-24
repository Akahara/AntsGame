#include "CMaze.h"

/*
Default contructor for a maze, create a 2x2 maze with no inside wall, the nest on the bottom left and a food source on the top right
In : /
Preconditions : /
Out : /
Postcondition :
	- The maze has been successfully created OR
	- Error 1 : AllocationFailed
*/
CMaze::CMaze() {
	nbLine = 2;
	nbColumn = 2;
	nestLine = 2;
	nestColumn = 2;
	tiles = (CTile**) malloc(sizeof(CTile*) * nbLine * nbColumn);
	if (tiles == nullptr) {
		throw new CException(AllocationFailed);
	}
	tiles[0] = new CTile(true, false, true, false, true, false);
	tiles[1] = new CTile(true, false, false, true, false, false);
	tiles[2] = new CTile(false, true, true, false, false, false);
	tiles[3] = new CTile(false, true, false, true, false, true);
}


/*
Create a maze from parameters
In :
	- nbLineIn, nbColumnIn : uint32_t, size of the maze, both must be >= 2
	- nestLineIn, nestColumnIn : uint32_t, position of the nest in the maze, must respectively be < to nbLineIn and nbColumnIn
	- nbFood : uint32_t, number of food sources in the maze, must be <= to (nbLine * nbColumn) - (((nbLine / 2) - 1) * ((nbColumn / 2) - 1) * 2)
	- difficulty : uint32_t, difficulty of the maze
	- algoToUse : uint32_t, algorithm to use to generate the maze, liste avaiable in the CMazeGenerator.h file
Preconditions : /
Out : /
Postcondition :
	- The maze has been successfully created OR
	- Error 1 : AllocationFailed  OR
	- Error 2 : InvalidSize, the size of the maze is invalid OR
	- Error 3 : InvalidPosition, the position of the nest is not in the maze OR
	- Error 4 : TooMuchFood, ther is too much food for this size maze
*/
CMaze::CMaze(uint32_t nbLineIn, uint32_t nbColumnIn, uint32_t nestLineIn, uint32_t nestColumnIn, uint32_t nbFood, uint32_t difficulty, uint32_t algoToUse)
{
	if (nbLineIn < 2 || nbColumnIn < 2) {
		throw new CException(InvalidSize);
	}
	else if (nestLineIn >= nbLineIn || nestColumnIn >= nbColumnIn) {
		throw new CException(InvalidPosition);
	}
	else if (nbFood > (nbLine * nbColumn) - (((nbLine / 2) - 1) * ((nbColumn / 2) - 1) * 2)) {
		throw new CException(TooMuchFood);
	}

	nbLine = nbLineIn;
	nbColumn = nbColumnIn;
	nestLine = nestLineIn;
	nestColumn = nestColumnIn;
	tiles = (CTile**)malloc(sizeof(CTile*) * nbLine * nbColumn);
	if (tiles == nullptr) {
		throw new CException(AllocationFailed);
	}
	switch (algoToUse) {
		case 1:
			CMazeGenerator::generateEmptyMaze(this);
			break;
		case 2:
			CMazeGenerator::generateMazeWithDepthFirstAlgo(this, nbFood, difficulty);
			break;
		default:
			CMazeGenerator::generateEmptyMaze(this);
			break;
	}
}


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
CMaze::CMaze(const CMaze& maze) {
	nbLine = maze.nbLine;
	nbColumn = maze.nbColumn;
	nestLine = maze.nestLine;
	nestColumn = maze.nestColumn;
	tiles = (CTile**)malloc(sizeof(CTile*) * nbLine * nbColumn);
	if (tiles == nullptr) {
		throw new CException(AllocationFailed);
	}
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		tiles[i] = maze.tiles[i];
	}
}


/*
Destructor of the CMaze class, free the tiles
*/
CMaze::~CMaze() {
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		free(tiles[i]);
	}
	free(tiles);
}


/*
Return a pointer to a copy of the tile at the set coordinates
In :
	- line, column : uint32_t, coordinates of the tile, must be respectively < nbLine and nbColumn
Preconditions : /
Out :
	- CTile* : the copy of the tile
Postcondition :
	- The tile has been successfully returned OR
	- Error 1 : AllocationFailed OR
	- Error 3 : InvalidPosition, the coordinates of the tile is incorect
*/
CTile* CMaze::getTile(uint32_t line, uint32_t column) {
	if (line >= nbLine || column >= nbColumn) {
		throw new CException(InvalidPosition);
	}
	CTile* tileCopy = (CTile*) malloc(sizeof(CTile));
	if (tileCopy == nullptr) {
		throw new CException(AllocationFailed);
	}
	tileCopy = new CTile(*tiles[nbColumn * line + column]);
	return tileCopy;
}

/*
Replace a tile of the maze at the passed position with a copy of the one passed in parameter 
In :
	- line, column : uint32_t, coordinates of the tile, must be respectively < nbLine and nbColumn
Preconditions : /
Out : /
Postcondition :
	- The tile has been successfully replaced OR
	- Error 1 : AllocationFailed OR
	- Error 3 : InvalidPosition, the coordinates of the tile is incorect
*/
void CMaze::setTile(uint32_t line, uint32_t column, CTile* tile) {
	if (line >= nbLine || column >= nbColumn) {
		throw new CException(InvalidPosition);
	}
	//delete tiles[nbColumn * line + column];
	tiles[nbColumn * line + column] = new CTile(*tile);
}


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
Maze* CMaze::convertToMazeStruct() {
	uint8_t* tilesForStruct = (uint8_t*) malloc(sizeof(uint8_t) * nbLine * nbColumn);
	if (tilesForStruct == nullptr) {
		throw new CException(AllocationFailed);
	}
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		tilesForStruct[i] = tiles[i]->getTileValue();
	}
	Maze* maze = (Maze*) malloc(sizeof(Maze));
	*maze = { nbColumn, nbLine, nestColumn, nestLine, tilesForStruct };
	return maze;
}


/*
Print the maze in the console
F : Food, f : food with a wall under it
N : Nest, n : nest with a wall under it
*/
void CMaze::printMaze() {
	uint32_t line, column;
	std::cout << " ";
	for (column = 0; column < nbColumn; column++) {
		std::cout << "_ ";
	}
	std::cout << "\n";
	for (line = 0; line < nbLine; line++) {
		for (column = 0; column < nbColumn; column++) {
			if (tiles[nbColumn * line + column]->hasWallW()) {
				std::cout << "|";
			}
			else {
				std::cout << " ";
			}
			if (tiles[nbColumn * line + column]->hasWallS()) {
				if (tiles[nbColumn * line + column]->hasFood()) {
					std::cout << "f";
				}
				else if (tiles[nbColumn * line + column]->hasNest()) {
					std::cout << "n";
				}
				else {
					std::cout << "_";
				}
			}
			else if (tiles[nbColumn * line + column]->hasFood()) {
				std::cout << "F";
			}
			else if (tiles[nbColumn * line + column]->hasNest()) {
				std::cout << "N";
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << "|\n";
	}
}


/*
Create an identic maze from another one
In :
	- maze : CMaze, maze to copy
Preconditions : /
Out :
	- CMaze : copy of the passed maze
Postcondition :
	- The maze has been successfully created and returned OR
	- Error 1 : AllocationFailed
*/
CMaze CMaze::operator=(CMaze maze) {
	// Destrution of the actual maze
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		free(tiles[i]);
	}
	free(tiles);
	// Copy of the passed maze
	nbLine = maze.nbLine;
	nbColumn = maze.nbColumn;
	nestLine = maze.nestLine;
	nestColumn = maze.nestColumn;
	tiles = (CTile**)malloc(sizeof(CTile*) * nbLine * nbColumn);
	if (tiles == nullptr) {
		throw new CException(AllocationFailed);
	}
	for (uint32_t i = 0; i < nbLine * nbColumn; i++) {
		tiles[i] = maze.tiles[i];
	}
	return *this;
}


EXPORT_C CMaze* CMaze_new(void) {
	return new CMaze();
}

EXPORT_C CMaze* CMaze_new_with_param(uint32_t nbLineIn, uint32_t nbColumnIn, uint32_t nestLineIn, uint32_t nestColumnIn, uint32_t nbFood, uint32_t difficulty, uint32_t algoToUse) {
	return new CMaze(nbLineIn, nbColumnIn, nestLineIn, nestColumnIn, nbFood, difficulty, algoToUse);
}

EXPORT_C void CMaze_delete(CMaze* maze) {
	delete maze;
}


EXPORT_C uint32_t CMaze_getNbLine(CMaze* maze) {
	return maze->getNbLine();
}

EXPORT_C uint32_t CMaze_getNbColumn(CMaze* maze) {
	return maze->getNbColumn();
}

EXPORT_C uint32_t CMaze_getNestLine(CMaze* maze) {
	return maze->getNestLine();
}

EXPORT_C uint32_t CMaze_getNestColumn(CMaze* maze) {
	return maze->getNestColumn();
}

EXPORT_C CTile* CMaze_getTile(CMaze* maze, uint32_t line, uint32_t column) {
	return maze->getTile(line, column);
}

EXPORT_C void CMaze_setTile(CMaze* maze, uint32_t line, uint32_t column, CTile* tile) {
	maze->setTile(line, column, tile);
}

EXPORT_C Maze* CMaze_convertToMazeStruct(CMaze* maze) {
	return maze->convertToMazeStruct();
}


EXPORT_C void CMaze_printMaze(CMaze* maze) {
	maze->printMaze();
}