/**
 * @file game.h
 * @brief Header file for the `game` class.
 */

#pragma once

#include <functional>
#include <iostream>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <memory>
#include "../JSON.h"
#include "../Maze/libAntMaze.h"
#include "session.h"
#include "player.h"
#include "constants.h"

 /**
  * @class game
  * @brief A class representing a game.
  */
class game
{
private:
    /**
     * @brief The difficulty of the game.
     */
    int difficulty;

    /**
     * @brief A vector of players in the game.
     */
    std::vector<Player> p_players;

    /**
     * @brief The maximum number of players allowed in the game.
     */
    int MAX_PLAYERS;

    /**
     * @brief The current number of players in the game.
     */
    int p_actual_players;

    /**
     * @brief A vector of pheromons in the game.
     */
    std::vector<float> p_pheromons;

    /**
     * @brief The number of tiles in the maze.
     */
    int numberOfTiles;

    /**
     * @brief A pointer to the maze in the game.
     */
    Maze* p_Maze;

public:
    /**
     * @brief Constructs a new game.
     * @param _difficulty The difficulty of the game.
     * @param _max_nb_players The maximum number of players allowed in the game.
     * @param size_side_maze The size of the side of the maze.
     */
    game(const int& _difficulty, const int& _max_nb_players, int size_side_maze);

    /**
     * @brief Destructs the game (more exactly the maze)
     */
    ~game();

    /**
     * @brief Adds a player to the game.
     * @param _player_uuid The UUID of the player.
     * @param _session The session of the player.
     */
    void join(const boost::uuids::uuid& _player_uuid, std::shared_ptr<session> _session);

    /**
     * @brief Moves a player in the game.
     * @param _player The UUID of the player.
     * @param _move The move to make.
     */
    void move(const boost::uuids::uuid& _player, std::string _move);

    /**
     * @brief Remove a player from the game after he disconnects.
     * @param _session The session attached to the player.
     */
    void remove(std::shared_ptr<session> _session);
    
    /**
     * @brief End the current game by calling the destructor
     */
    void endGame();

    /**
     * @brief Decreases the value of all pheromons in the game, and send the result to all players connected.
     */
    void decreasePheromons();

    /**
     * @brief Gets the pheromons in the game.
     * @return A vector of pheromons in the game.
     */
    std::vector<float> getPheromons() { return p_pheromons; };

    /**
   * @brief Gets the maze in the game.
   * @return A pointer to the maze in the game.
   */
    Maze* getMaze() { return p_Maze; };

    /**
     * @brief Gets the maximum number of players allowed in the game.
     * @return The maximum number of players allowed in the game.
     */
    int getMax_Players() { return MAX_PLAYERS; };

    /**
     * @brief Gets the current number of players in the game.
     * @return The current number of players in the game.
     */
    int getNb_Players() { return p_actual_players; };


};