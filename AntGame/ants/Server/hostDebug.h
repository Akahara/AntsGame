#pragma once

#include "Client/client.h"
#include "Server/server.h"

#include <thread>
#include <chrono>

namespace Server {

	static void startLocalServer() {


        JSON::LoadOptionFile("../AntGame/ants/Server/options.json");

        boost::asio::io_context context{};

        server server{ context, (unsigned short)88000 };

        // -------------------------------------------------------------------------

        auto t1 = std::thread([&]
            {
                context.run();
            });

        // -------------------------------------------------------------------------

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            //update all games and send pheromons vector to all connected players
            for (game* game : server.getListofAvailaibleGames()) {
                game->decreasePheromons();
            }
        }

        // -------------------------------------------------------------------------

	}

    /**
    * Allocates new memory, dont forget to delete client
    */
    static void setClientConnexion(Client* client) {

        std::string adress = "192.168.1.23";
        unsigned short port = 88000;
        boost::asio::io_context io_context1;

       // Client client1{ io_context1, adress, port };
        client = new Client(io_context1, adress, port);

    }

}