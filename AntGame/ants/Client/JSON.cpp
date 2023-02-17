#include "json.hpp"
#include "JSON.h"
#include <cstring>
#include <optional>

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

using json = nlohmann::json;

std::string JSON::getUUID(json &root) {
  return root["body"]["playerId"].get<std::string>();
}

std::string JSON::getType(json &root) {
  return root["type"].get<std::string>();
}

Maze JSON::getMaze(json &root) {
  Maze maze = {0};

  maze.nbColumn = root["body"]["maze"]["nbColumn"].get<uint32_t>();
  maze.nbLine = root["body"]["maze"]["nbLine"].get<uint32_t>();
  maze.nestColumn = root["body"]["maze"]["nestColumn"].get<uint32_t>();
  maze.nestLine = root["body"]["maze"]["nestLine"].get<uint32_t>();

  maze.tiles = new uint8_t[maze.nbLine * maze.nbColumn];

  auto vec = root["body"]["maze"]["tiles"].get<std::vector<uint8_t>>();

  memcpy(maze.tiles, vec.data(), maze.nbLine * maze.nbColumn);

  return maze;
}

std::vector<float> JSON::getPheromons(json &root) {
  return root["body"]["pheromon"].get<std::vector<float>>();
}

std::vector<uint8_t> JSON::finish(json &root, std::optional<std::string> uuid) {
  if (uuid.has_value())
    root["body"]["playerId"] = *uuid;

  std::string str = root.dump();
  std::vector<uint8_t> output(sizeof(uint32_t) + str.length());

  uint32_t length_bom = htonl(str.length());
  unsigned char *length_bytes = (unsigned char *)&length_bom;

  memcpy(output.data(), length_bytes, sizeof(uint32_t));
  memcpy(output.data() + sizeof(uint32_t), str.data(), str.length());

  return output;
}

std::vector<uint8_t> JSON::createJoin(std::optional<std::string> uuid, int difficulty) {
  json root({
		{"type", "join"},
		{"body", {
			{"difficulty", (unsigned int)difficulty }
			} }
	});
  return finish(root, uuid);
}

std::vector<uint8_t> JSON::createMove(std::string uuid, int _move) {
	json root({
		{ "type", "move" },
		{ "body", {
			{ "direction", _move }
		} }
	});

	return finish(root, uuid);
}
