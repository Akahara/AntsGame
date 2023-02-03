// Labyrinthe.cpp : Définit les fonctions de la bibliothèque statique.
//

#include "libAntMaze.h"
#include "CMaze.h"



/*
Fonction that test the library
*/
/* extern "C" void mazeTest() {
	CMaze maze;
	CMaze maze2(5, 5, 5, 5, 1, 1, 1);
	Maze* mazeS1 = maze2.convertToMazeStruct();

	std::cout << "\n";
	for (int i = 0; i < mazeS1->nbLine * mazeS1->nbColumn; i++) {
		std::cout << (unsigned int) mazeS1->tiles[i] << " ";
	}
	std::cout << "\nFin de la fontion mazeTest\n";
} */


EXPORT_C Maze* generateMaze(const ParamMaze* param) {
	uint32_t algoToUse = 2; // A modifier pour changer l'algo de génération de labyrinthe

	uint32_t nbColumn = param->nbColumn;
	uint32_t nbLine = param->nbLine;
	uint32_t nestColumn = param->nestColumn;
	uint32_t nestLine = param->nestLine;
	uint32_t nbFood = param->nbFood;
	uint32_t difficulty = param->difficulty;
	CMaze* mazeClass = CMaze_new_with_param(nbLine, nbColumn, nestLine, nestColumn, nbFood, difficulty, algoToUse);
	Maze* mazeStruct = CMaze_convertToMazeStruct(mazeClass);
	//mazeClass->printMaze();
	return mazeStruct;
	
	//return nullptr;
}

EXPORT_C void freeMaze(Maze** maze) {
	if (*maze != nullptr) {
		free((*maze)->tiles);
	}
	free(*maze);
}