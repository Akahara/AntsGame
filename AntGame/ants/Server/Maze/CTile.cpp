#include "CTile.h"


/*
Default constructor, create a tile with 4 walls, no food and no nest
*/
CTile::CTile() {
	wallN = true;
	wallS = true;
	wallW = true;
	wallE = true;
	food = false;
	nest = false;
}

CTile::CTile(bool hasWallN, bool hasWallS, bool hasWallW, bool hasWallE, bool hasFood, bool hasNest) {
	wallN = hasWallN;
	wallS = hasWallS;
	wallW = hasWallW;
	wallE = hasWallE;
	food = hasFood;
	nest = hasNest;
}

/*
Constructor that create an identic CTile from another one
*/
CTile::CTile(const CTile& tile) {
	wallN = tile.wallN;
	wallS = tile.wallS;
	wallW = tile.wallW;
	wallE = tile.wallE;
	food = tile.food;
	nest = tile.nest;
}

/*
Return the value of the tile in the Maze Struct
*/
uint8_t CTile::getTileValue() {
	uint8_t res = 0;
	if (wallN) {
		res += 1;
	}
	if (wallE) {
		res += 2;
	}
	if (wallS) {
		res += 4;
	}
	if (wallW) {
		res += 8;
	}
	if (food) {
		res += 16;
	}
	if (nest) {
		res += 32;
	}
	return res;
}

/*
Operator = that create and return an identic CTile from another one
*/
CTile CTile::operator=(CTile tile) {
	wallN = tile.wallN;
	wallS = tile.wallS;
	wallW = tile.wallW;
	wallE = tile.wallE;
	food = tile.food;
	nest = tile.nest;
	return *this;
}



EXPORT_C CTile* CTile_new(void) {
	return new CTile();
}

EXPORT_C CTile* CTile_new_with_param(bool hasWallN, bool hasWallS, bool hasWallW, bool hasWallE, bool hasFood, bool hasNest) {
	return new CTile(hasWallN, hasWallS, hasWallW, hasWallE, hasFood, hasNest);
}

EXPORT_C CTile* CTile_new_by_copy(CTile* tile) {
	return new CTile(*tile);
}

EXPORT_C void CTile_delete(CTile* tile) {
	delete tile;
}

EXPORT_C bool CTile_hasWallN(CTile* tile) {
	return tile->hasWallN();
}

EXPORT_C bool CTile_hasWallS(CTile* tile) {
	return tile->hasWallS();
}

EXPORT_C bool CTile_hasWallW(CTile* tile) {
	return tile->hasWallW();
}

EXPORT_C bool CTile_hasWallE(CTile* tile) {
	return tile->hasWallE();
}

EXPORT_C bool CTile_hasFood(CTile* tile) {
	return tile->hasFood();
}

EXPORT_C bool CTile_hasNest(CTile* tile) {
	return tile->hasNest();
}

EXPORT_C void CTile_setWallN(CTile* tile, bool hasWallN) {
	tile->setWallN(hasWallN);
}

EXPORT_C void CTile_setWallS(CTile* tile, bool hasWallS) {
	tile->setWallS(hasWallS);
}

EXPORT_C void CTile_setWallW(CTile* tile, bool hasWallW) {
	tile->setWallW(hasWallW);
}

EXPORT_C void CTile_setWallE(CTile* tile, bool hasWallE) {
	tile->setWallE(hasWallE);
}

EXPORT_C void CTile_setFood(CTile* tile, bool hasFood) {
	tile->setFood(hasFood);
}
	
EXPORT_C void CTile_setNest(CTile* tile, bool hasNest) {
	tile->setNest(hasNest);
}

EXPORT_C uint8_t CTile_getTileValue(CTile* tile){
	return tile->getTileValue();
}