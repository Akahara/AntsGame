#pragma once

#include "Client/client.h"
#include "Server/server.h"

#include <thread>
#include <chrono>

namespace Server {

    static boost::asio::io_context* context = new boost::asio::io_context();

	static server* startLocalServer() {


        JSON::LoadOptionFile("../AntGame/ants/Server/options.json"); // todo maybe this isnt found

        server* s = new server( *context, (unsigned short)88000 );

        // -------------------------------------------------------------------------
        
        std::thread([=]
            {
                context->run();
            }).detach();
        
        // -------------------------------------------------------------------------

        
        std::thread([=] {


                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            while (true) {

                //update all games and send pheromons vector to all connected players
                //std::cout << "server sent infos" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                for (game* game : s->getListofAvailaibleGames()) 
                    game->decreasePheromons();
                   
                
                }
            }
        ).detach();
        
        return s;

        // -------------------------------------------------------------------------

	}

    /**
    * Allocates new memory, dont forget to delete client
    */
    static Client* setClientConnexion() {

        std::string adress = "127.0.0.1";
        unsigned short port = 88000;
        static boost::asio::io_context io_context1;

       // Client client1{ io_context1, adress, port };
        return new Client(io_context1, adress, port);

    }

}