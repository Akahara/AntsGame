#include "session.h"
#include "../JSON.h"

#include <functional>
#include <iostream>
#include <thread>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp> // generators


#include "server.h"
#include "Game.h"

using boost::asio::async_write;
using boost::asio::buffer;
using boost::asio::io_context;
using boost::asio::error::connection_reset;
using boost::asio::error::eof;
using boost::system::error_code;
using boost::system::system_error;

using std::placeholders::_1;
using std::placeholders::_2;


session::session(io_context& service, server* _server) : p_socket{ service }, p_origin{ _server } {
    
}

void session::listen() {
    
    auto handler = std::bind(&session::handle_read, shared_from_this(), _1, _2);
     
    // We listen until a # appears, signification of message's end.
    boost::asio::async_read_until(p_socket, buffer, '#', handler);
}

void session::handle_write(const std::error_code& ec)
{
    
}

void session::handle_read(const error_code& ec, size_t bytes_transferred) {
    if (ec) {
        if (ec == eof || ec == connection_reset) {
            return;
        }

        throw system_error{ ec };
    }

    // Extract the data from buffer to a const buffer
    boost::asio::streambuf::const_buffers_type bufs = buffer.data();
    buffer.consume(2048);

    // Create a string from the const buffer
    std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + bytes_transferred);
    
    // Remove the #
    std::string string = str.substr(8, bytes_transferred - 9);    
    
    if (Constants::VERBOSE) {
        std::cout << "serveur recoit : " << string << "\n";
        std::cout << bytes_transferred << ";;" << string.length() << "\n";
    }
    
    // Create the property tree to parse the JSON
    boost::property_tree::ptree JSON_Property_Tree;
    
    // Put the data received in a strinstream to parse it
    std::stringstream ss;
    ss << string;
    boost::property_tree::read_json(ss, JSON_Property_Tree);

    
    // Extract the type of message
    std::string type = JSON::getType(JSON_Property_Tree);
    
    if (type == "join"){
        
        boost::uuids::uuid UUID = boost::lexical_cast<boost::uuids::uuid>(JSON::getUUID(JSON_Property_Tree));
        
        // If UUID is nil, that means it's a new player, and not a reconnexion
        if (UUID.is_nil()) {

            std::cout << "new player joins" << std::endl;

            // Generate a UUID for the player
            UUID = boost::uuids::random_generator()();

            // Extract the difficulty wanted for the game
            int difficulty = JSON::getDifficultyJoin(JSON_Property_Tree);

            // FInd a gam� with this difficulty
            p_origin->findGameWithDifficulty(difficulty, UUID, shared_from_this());
        }
        else {
            // If we,re here, that means it's a reconnexion. 
            // We just set the game of the session where he was previously connected
            p_game = p_origin->getGame(UUID);
        }
        
        
        
    }
    else if(type == "move") {
        
        // Extrac UUID and move of the player
        boost::uuids::uuid UUID = boost::lexical_cast<boost::uuids::uuid>(JSON::getUUID(JSON_Property_Tree));
        std::string MOVE = JSON::getMove(JSON_Property_Tree);
        
        // Add the move to the actual game
        p_game->move(UUID, MOVE);
        
        // Send pheromons back
        sendPheromons(UUID, p_game->getPheromons());

        std::cout << "Player : " << UUID << " made a movement : " << MOVE << "\n";
        
    }
    listen();
}

void session::sendMaze(boost::uuids::uuid _uuid,Maze* _maze)
{
    // Create the JSON with the maze and send it
    std::string JSONokMaze = JSON::createokMaze(_uuid, *_maze) + "#";
    sendString(JSONokMaze);
}

void session::sendPheromons(boost::uuids::uuid _uuid,const std::vector<float>& _pheromons)
{
    // Create the JSON with the pheromons and send it
    std::string JSONInfo = JSON::createInfo(_uuid, _pheromons) + "#";
    sendString(JSONInfo);
}

void session::sendString(std::string _message) {

    if (Constants::VERBOSE) {
        std::cout << "serveur envoi " << _message << std::endl;
        std::cout << "taille du paquet " << _message.size() << std::endl;
    }

    boost::system::error_code error;
    boost::asio::write(p_socket, boost::asio::buffer(_message), error);
    if (error) {
        if (Constants::VERBOSE) {
            std::cout << "send failed: " << error.message() << std::endl;
        }
        p_game->remove(shared_from_this());
    }
}



