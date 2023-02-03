/**
 * @file server.h
 * @brief Header file for the `server` class.
 */

#pragma once

#include <vector>
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>  
#include <boost/uuid/uuid_io.hpp> 


#include "game.h"
#include "constants.h"


/**
 * @class server
 * @brief A class representing a server that manages games and sessions.
 */
class server {
    using acceptor_t = boost::asio::ip::tcp::acceptor;
    using endpoint_t = boost::asio::ip::tcp::endpoint;
    using socket_t = boost::asio::ip::tcp::socket;


public:
    /**
     * @brief Constructs a new server.
     * @param service The IO context to use for the server.
     * @param port The port to listen on.
     */
    server(boost::asio::io_context& service, unsigned short port);

    /**
     * @brief Starts accepting new connections, and send all new connection in new sessions.
     */
    void startAccept();


    /**
     * @brief Handles the acceptance of a new session.
     * @param new_session The new session.
     * @param ec The error code of the operation.
     */
    void handleAccept(std::shared_ptr<session> new_session,
        const boost::system::error_code& ec);

    /**
     * @brief Attempts to matchmake a player with a game of the specified difficulty.
     * @param _difficulty The desired difficulty of the game.
     * @param _uuid The UUID of the player.
     * @param _session The session of the player.
     */
    void findGameWithDifficulty(int _difficulty, boost::uuids::uuid _uuid, std::shared_ptr<session> _session);

    /**
     * @brief Returns the game where the given UUID is connected
     * @param _uuid The UUID of the game to get.
     * @return A pointer to the game with the specified UUID.
     */
    game* getGame(const boost::uuids::uuid& _uuid);

    /**
     * @brief Returns a list of available games.
     * @return A vector of `game` objects.
     */
    std::vector<game*> getListofAvailaibleGames() { return p_games; };

private:
    /**
     * @brief A vector of games where there is space to new players.
     */
    std::vector<game*> p_games;

    /**
     * @brief A vector of pairs mapping player UUIDs to the games they are in.
     */
    std::vector <std::pair<boost::uuids::uuid, game*>> p_players_game;

    /**
     * @brief The IO context of the server.
     */
    boost::asio::io_context& p_context;

    /**
     * @brief The acceptor used to accept new connections.
     */
    acceptor_t p_acceptor;    
};