/**
 * @file client.h
 * @brief Header file for the `Client` class.
 */

#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <memory>
#include <boost/uuid/uuid.hpp>   
#include "../AntGame/ants/Maze/MazeGeneration_proj/libAntMaze.h"
#include "../JSON.h"

#define NULL_UUID    { 00000000 - 0000 - 0000 - 0000 - 000000000000 }

using boost::asio::ip::tcp;


namespace Server {

    /**
     * @class Client
     * @brief A class representing a client.
     */
    class Client : public std::enable_shared_from_this<Client> {
    public:
        enum MOVE_LIST {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            WTF
        };

    public:
        /**
         * @brief Constructs a new client.
         * @param io_context1 The IO context to use for the client.
         * @param _adress The address of the server to connect to.
         * @param _port The port of the server to connect to.
         */
        Client(boost::asio::io_context& io_context1, std::string _adress, unsigned short _port);

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
        void move(std::string _move);
        void move(MOVE_LIST direction) {
            std::string moves[4] = {"haut","bas","gauche", "droite"};
            move(moves[direction]);
        }

        /**
         * @brief Handles the completion of a read operation.
         * @param ec The error code of the operation.
         * @param bytes_transferred The number of bytes transferred.
         */
        void handleReadClient(const boost::system::error_code& ec,
            size_t bytes_transferred);

        /**
         * @brief Listens for incoming messages.
         */
        void listenClient();

        /**
         * @brief Sets the maze of the client.
         * @param _maze A pointer to the maze to set.
         */
        void setMaze(Maze* _maze) { p_maze = _maze; }
    
        Maze* getMaze() { return p_maze; }
        std::vector<float> getPheromones() const { return p_pheromons; }

private:
        /**
         * @brief The IO context of the client.
         */
        boost::asio::io_context& p_io_context;

        /**
         * @brief The socket of the client.
         */
        tcp::socket p_socket_client;

        /**
         * @brief The UUID of the client.
         */
        boost::uuids::uuid p_uuid;

        /**
         * @brief A buffer for incoming messages.
         */
        boost::asio::streambuf p_buffer{ 2048 };

        /**
         * @brief A pointer to the maze of the client.
         */
        Maze* p_maze;

        std::thread t1;

        std::vector<float> p_pheromons;
    };
}