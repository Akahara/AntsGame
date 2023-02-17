/**
 * @file client.h
 * @brief Header file for the `Client` class.
 */

#pragma once

#include "../AntGame/ants/Maze/MazeGeneration_proj/libAntMaze.h"
#include "ProtocolHandler.h"
#include <iostream>
#include <memory>
#include <optional>

#define NULL_UUID                                                              \
  { 00000000 - 0000 - 0000 - 0000 - 000000000000 }

namespace Server {

/**
 * @class Client
 * @brief A class representing a client.
 */
class Client {
public:
  enum MOVE_LIST { UP, DOWN, LEFT, RIGHT, WTF };

public:
  /**
   * @brief Constructs a new client.
   * @param _adress The address of the server to connect to.
   * @param _port The port of the server to connect to.
   */
  Client(std::string _adress, unsigned short _port);

  ~Client();

  /**
   * @brief Sends a join request to the server.
   * @param _difficulty The desired difficulty of the game.
   */
  void join(int _difficulty);

  /**
   * @brief Sends a move request to the server.
   * @param _move The move to make.
   */
  void move(MOVE_LIST direction);

  /**
   * @brief Listens for incoming messages.
   */
  void listenClient();

  std::optional<Maze> getMaze() { return maze; }
  std::vector<float> getPheromones() const { return p_pheromons; }

private:
  /**
   * @brief The socket file descriptor.
   */
  int fd;

  /**
   * @brief The UUID of the client.
   */
  std::string p_uuid;

  /**
   * @brief The maze of the client.
   */
  std::optional<Maze> maze;

  std::vector<float> p_pheromons;
  ProtocolHandler handler;
};
} // namespace Server