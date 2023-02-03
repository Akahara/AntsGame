#pragma once

#ifdef __cplusplus // If we use C++
#include <iostream>

class CTile
{

private:
	// True : have it, else false
	bool wallN, wallS, wallW, wallE, food, nest;


public:

	/*
	Default constructor, create a tile with 4 walls, no food and no nest
	*/
	CTile();

	/*
	Construct a tile with the passed parameters
	*/
	CTile(bool hasWallN, bool hasWallS, bool hasWallW, bool hasWallE, bool hasFood, bool hasNest);


	/*
	Constructor that create an identic CTile from another one
	*/
	CTile(const CTile& tile);
	

	// GETTEURS ET SETTEURS

public:

	bool hasWallN() {
		return wallN;
	}

	bool hasWallS() {
		return wallS;
	}

	bool hasWallW() {
		return wallW;
	}

	bool hasWallE() {
		return wallE;
	}

	bool hasFood() {
		return food;
	}

	bool hasNest() {
		return nest;
	}

	void setWallN(bool hasWallN) {
		wallN = hasWallN;
	}

	void setWallS(bool hasWallS) {
		wallS = hasWallS;
	}

	void setWallW(bool hasWallW) {
		wallW = hasWallW;
	}

	void setWallE(bool hasWallE) {
		wallE = hasWallE;
	}

	void setFood(bool hasFood) {
		food = hasFood;
	}
	
	void setNest(bool hasNest) {
		nest = hasNest;
	}


	// OTHER METHODS

public:

	/*
	Return the value of the tile in the Maze Struct
	*/
	uint8_t getTileValue();


	// OPERATORS

public:

	/*
	Operator = that create and return an identic CTile from another one
	*/
	CTile operator=(CTile tile);
};


// #ifdef __cplusplus

#else // If we use C

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct CTile {
	bool wallN, wallS, wallW, wallE, food, nest;
} CTile;

#endif



// access functions
#ifdef __cplusplus
    #define EXPORT_C extern "C"
#else
    #define EXPORT_C
#endif


EXPORT_C CTile* CTile_new(void);

EXPORT_C CTile* CTile_new_with_param(bool hasWallN, bool hasWallS, bool hasWallW, bool hasWallE, bool hasFood, bool hasNest);

EXPORT_C CTile* CTile_new_by_copy(CTile* tile);

EXPORT_C void CTile_delete(CTile* tile);

EXPORT_C bool CTile_hasWallN(CTile* tile);

EXPORT_C bool CTile_hasWallS(CTile* tile);

EXPORT_C bool CTile_hasWallW(CTile* tile);

EXPORT_C bool CTile_hasWallE(CTile* tile);

EXPORT_C bool CTile_hasFood(CTile* tile);

EXPORT_C bool CTile_hasNest(CTile* tile);

EXPORT_C void CTile_setWallN(CTile* tile, bool hasWallN);

EXPORT_C void CTile_setWallS(CTile* tile, bool hasWallS);

EXPORT_C void CTile_setWallW(CTile* tile, bool hasWallW);

EXPORT_C void CTile_setWallE(CTile* tile, bool hasWallE);

EXPORT_C void CTile_setFood(CTile* tile, bool hasFood);
	
EXPORT_C void CTile_setNest(CTile* tile, bool hasNest);

EXPORT_C uint8_t CTile_getTileValue(CTile* tile);