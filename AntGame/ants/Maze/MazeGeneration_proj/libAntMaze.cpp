// Labyrinthe.cpp : Définit les fonctions de la bibliothèque statique.
//

#include "libAntMaze.h"
#include "CMaze.h"


Maze* generateMazeCPP(const ParamMaze* param) {
	try {
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
	}
	catch (CException e) {
		lastError = e.getValeur();
		return nullptr;
	}
}

void freeMazeCPP(Maze** maze) {
	if (*maze != nullptr) {
		free((*maze)->tiles);
	}
	free(*maze);
}




EXPORT_C Maze* generateMaze(const ParamMaze* param) {
	return generateMazeCPP(param);
}

EXPORT_C void freeMaze(Maze** maze) {
	freeMazeCPP(maze);
}