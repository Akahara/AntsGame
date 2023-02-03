/**
 * @file session.h
 * @brief Header file for the `session` class.
 */

#pragma once

#include <array>
#include <memory>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

class game;
class server;
class Maze;

/**
 * @class session
 * @brief A class representing a session between a client and the server.
 */
class session : public std::enable_shared_from_this<session> {

    using endpoint_t = boost::asio::ip::tcp::endpoint;
    using socket_t = boost::asio::ip::tcp::socket;

public:
    /**
     * @brief Constructs a new session.
     * @param service The IO context to use for the session.
     * @param _server A pointer to the server that created the session.
     */
    session(boost::asio::io_context& service, server* _server);

    /**
     * @brief Listens for incoming messages.
     */
    void listen();

    /**
     * @brief Handles the completion of a write operation.
     * @param ec The error code of the operation.
     */
    void handle_write(const std::error_code& ec);

    /**
     * @brief Handles the completion of a read operation.
     * @param ec The error code of the operation.
     * @param bytes_transferred The number of bytes transferred.
     */
    void handle_read(const boost::system::error_code& ec,
        size_t bytes_transferred);

    /**
     * @brief Sends the maze to the client.
     * @param _uuid The UUID of the player.
     * @param _maze A pointer to the maze to send.
     */
    void sendMaze(boost::uuids::uuid _uuid, Maze* _maze);

    /**
     * @brief Sends the pheromons to the client.
     * @param _uuid The UUID of the player.
     * @param _pheromons A vector of pheromons to send.
     */
    void sendPheromons(boost::uuids::uuid _uuid, const std::vector<float>& _pheromons);

    /**
     * @brief Sends a string message to the client.
     * @param _message The message to send.
     */
    void sendString(std::string _message);

    /**
     * @brief Gets the socket of the session.
     * @return The socket of the session.
     */
    socket_t& socket() { return p_socket; };

    

    /**
  * @brief Set the game object for the current instance
  *
  * @param _game Pointer to the game object to be set
  */
    void setGame(game* _game) { p_game = _game; };

private:
    /**
     * @brief The socket of the session.
     */
    socket_t p_socket;

    /**
     * @brief A pointer to the server that created the session.
     */
    server* p_origin;

    /**
     * @brief A pointer to the game where the player of this session play.
     */
    game* p_game;

    /**
     * @brief A buffer for incoming messages.
     */
    boost::asio::streambuf buffer{ 2048 };
};