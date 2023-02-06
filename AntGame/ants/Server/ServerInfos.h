#pragma once

#include <vector>
#include "Client/client.h"
#include "Server/server.h"

namespace Server {

struct ServerInfos {

	uint32_t nbColumn;
	uint32_t nbLine;
	uint32_t nestColumn;
	uint32_t nestLine;
	uint8_t* tiles; // c + l * nbColumn 

	std::vector<float> pheromonesValues;


};

static ServerInfos pullServerInfos() 

{
	// mock
	// get the infos

	ServerInfos res;
	
	for (int i = 0; i < 20 * 20; i++) {
		res.pheromonesValues.push_back((float)rand()/(float)RAND_MAX);
	}

	return res;
}

static void increaseTilePheromone(const glm::uvec2& pos, const server& server)  
{
	// somehow send a request to update a certain tile quantity
}



static Server::Client::MOVE_LIST computeMoveDirection(const glm::uvec2& old_tile, const glm::uvec2& new_tile) {

	glm::uvec2 delta = old_tile - new_tile;
	// delta y = -1 --> up 0 (0, 1)
	// delta y = 1 --> down 1 (0, -1)
	// delta x = 1 --> left 2 (1, 0)
	// delta x = -1 --> right 3 (-1, 0)

	switch (delta.y) {
		case 0:
			break;
		case 1:
			return Server::Client::MOVE_LIST::UP;

		case -1:
			return Server::Client::MOVE_LIST::DOWN;
	}

	switch (delta.x) {
		case 1:
			return Server::Client::MOVE_LIST::LEFT;

		case -1:
			return Server::Client::MOVE_LIST::RIGHT;
	}
	return Server::Client::MOVE_LIST::WTF; // how
}

}