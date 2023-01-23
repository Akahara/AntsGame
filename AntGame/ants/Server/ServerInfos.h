#pragma once

#include <vector>

namespace Server {

struct ServerInfos {

	uint32_t nbColumn;
	uint32_t nbLine;
	uint32_t nestColumn;
	uint32_t nestLine;
	uint8_t* tiles; // c + l * nbColumn 


};

static ServerInfos pullServerInfos() 

{
	// mock
	// get the infos

}
}