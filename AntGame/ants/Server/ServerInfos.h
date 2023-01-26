#pragma once

#include <vector>

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

static void sendInfos(const glm::uvec2& pos) 
{
	std::cout << "Position sent : " << pos <<std::endl;

}

}