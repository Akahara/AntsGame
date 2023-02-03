#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Maze {
	uint32_t nbColumn;
	uint32_t nbLine;
	uint32_t nestColumn;
	uint32_t nestLine;
	uint8_t* tiles; // c + l * nbColumn
} Maze;

typedef struct ParamMaze {
	uint32_t nbColumn;
	uint32_t nbLine;
	uint32_t nestColumn;
	uint32_t nestLine;
	uint32_t nbFood;
	uint32_t difficulty;
} ParamMaze;

#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif

EXPORT_C Maze* generateMaze(const ParamMaze* param);
EXPORT_C void freeMaze(Maze** maze);

// Error management ?
// enum Error getLastError(void)
// char *getErrorMessage(void)