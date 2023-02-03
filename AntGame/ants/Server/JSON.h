/**
 * @file JSON.h
 * @brief Header file for the `JSON` namespace.
 */

#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>
#include <iostream>
#include "Maze/libAntMaze.h"

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
    std::string getUUID(const boost::property_tree::ptree& root);

    /**
     * @brief Returns the type from a JSON message.
     * @param root The root of the JSON message.
     * @return The type as a string.
     */
    std::string getType(const boost::property_tree::ptree& root);

    /**
     * @brief Returns the difficulty from a JSON join message.
     * @param root The root of the JSON message.
     * @return The difficulty as an integer.
     */
    int getDifficultyJoin(const boost::property_tree::ptree& root);

    /**
     * @brief Returns the move from a JSON move message.
     * @param root The root of the JSON message.
     * @return The move as a string.
     */
    std::string getMove(const boost::property_tree::ptree& root);

    /**
     * @brief Returns the maze from a JSON maze message.
     * @param root The root of the JSON message.
     * @return A pointer to the maze.
     */
    Maze* getMaze(const boost::property_tree::ptree& root);

    /**
     * @brief Returns the pheromons from a JSON info message.
     * @param root The root of the JSON message.
     * @return A vector of pheromons.
     */
    std::vector <float> getPheromons(const boost::property_tree::ptree& root);

    /**
     * @brief Loads the options from a JSON file.
     * @param _path The path to the file.
     */
    void LoadOptionFile(std::string _path);

    /**
     * @brief Creates a generic JSON message.
     * @param _uuid The UUID of the sender.
     * @param _root The root of the JSON message.
     * @return The JSON message as a string.
     */
    std::string createGeneric(boost::uuids::uuid _uuid,boost::property_tree::ptree& _root);

    /**
 * @brief Creates a JSON join message.
 * @param _uuid The UUID of the sender.
 * @param _difficulty The difficulty of the game.
 * @return The JSON message as a string.
 */
    std::string createJoin(boost::uuids::uuid _uuid, int _difficulty);

    /**
     * @brief Creates a JSON move message.
     * @param _uuid The UUID of the sender.
     * @param _move The move to be sent.
     * @return The JSON message as a string.
     */
    std::string createMove(boost::uuids::uuid _uuid, std::string _move);

    /**
     * @brief Creates a JSON maze message.
     * @param _uuid The UUID of the sender.
     * @param _maze The maze to be sent.
     * @return The JSON message as a string.
     */
    std::string createokMaze(boost::uuids::uuid _uuid, Maze _maze);

    /**
     * @brief Creates a JSON info message.
     * @param _uuid The UUID of the sender.
     * @param _pheromons The pheromons to be sent.
     * @return The JSON message as a string.
     */
    std::string createInfo(boost::uuids::uuid _uuid, std::vector<float> _pheromons);

}