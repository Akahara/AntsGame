/**
 * @file JSON.h
 * @brief Header file for the `JSON` namespace.
 */

#pragma once

#include <optional>
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include "../Maze/MazeGeneration_proj/libAntMaze.h"

#include "json.hpp"

using json = nlohmann::json;

 /**
  * @namespace JSON
  * @brief A namespace containing functions for reading and creating JSON messages.
  */
namespace JSON {

    /**
     * @brief Returns the UUID from a JSON message.
     * @param root The root of the JSON message.
     * @return The UUID as a string.
     */
    std::string getUUID(json &root);

    /**
     * @brief Returns the type from a JSON message.
     * @param root The root of the JSON message.
     * @return The type as a string.
     */
    std::string getType(json &root);

    /**
     * @brief Returns the move from a JSON move message.
     * @param root The root of the JSON message.
     * @return The move.
     */
    int getMove(json &root);

    /**
     * @brief Returns the maze from a JSON maze message.
     * @param root The root of the JSON message.
     * @return A pointer to the maze.
     */
    Maze getMaze(json &root);

    /**
     * @brief Returns the pheromons from a JSON info message.
     * @param root The root of the JSON message.
     * @return A vector of pheromons.
     */
    std::vector<float> getPheromons(json &root);

    /**
     * @brief Creates a generic JSON message.
     * @param root The root of the JSON message.
     * @param uuid The UUID of the sender.
     * @return The JSON message as a string.
     */
    std::vector<uint8_t> finish(json &root, std::optional<std::string> uuid);

    /**
 * @brief Creates a JSON join message.
 * @param _uuid The UUID of the sender.
 * @param _difficulty The difficulty of the game.
 * @return The JSON message as a string.
 */
    std::vector<uint8_t> createJoin(std::optional<std::string> uuid, int _difficulty);

    /**
     * @brief Creates a JSON move message.
     * @param _uuid The UUID of the sender.
     * @param _move The move to be sent.
     * @return The JSON message as a string.
     */
    std::vector<uint8_t> createMove(std::string _uuid, int _move);
}