/**
 * @file player.h
 * @brief Header file for the `Player` struct.
 */

#pragma once

#include <boost/uuid/uuid.hpp>
#include "session.h"

 /**
  * @struct Player
  * @brief A struct representing a player.
  */
struct Player {

    /**
     * @brief The UUID of the player.
     */
    boost::uuids::uuid p_uuid;

    /**
     * @brief The current column of the player.
     */
    int actual_column;

    /**
     * @brief The current line of the player.
     */
    int actual_line;

    /**
     * @brief A flag indicating if the player has food.
     */
    bool has_food;

    /**
     * @brief A shared pointer to the session of the player.
     */
    std::shared_ptr<session> _session;
};

