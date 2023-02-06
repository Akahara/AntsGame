/**
 * @file player.h
 * @brief Header file for the `Player` struct.
 */

#pragma once

#include <boost/uuid/uuid.hpp>
#include "session.h"

namespace Server {


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
    int actual_column = 0;

    /**
     * @brief The current line of the player.
     */
    int actual_line = 0;

    /**
     * @brief A flag indicating if the player has food.
     */
    bool has_food = false;

    /**
     * @brief A shared pointer to the session of the player.
     */
    std::shared_ptr<session> _session;
};


}