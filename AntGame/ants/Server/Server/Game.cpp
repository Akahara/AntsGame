#include "Game.h"
#include <iostream>
#include <stdlib.h>
#include <exception>


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

game::game(const int& _difficulty, const int& _max_nb_players, int _size_side_maze) : difficulty{ _difficulty }, MAX_PLAYERS{ _max_nb_players }
	{
		// Create the parametersMaze struct
		ParamMaze parameters_maze;

		if (_difficulty == 1) {
			parameters_maze.nbColumn = Constants::DIFFICULTY1_SIDE_SIZE;
			parameters_maze.nbLine = Constants::DIFFICULTY1_SIDE_SIZE;
			parameters_maze.nbFood = Constants::DIFFICULTY1_NBFOOD;
			parameters_maze.nestLine = Constants::DIFFICULTY1_NESTLINE;
			parameters_maze.nestColumn = Constants::DIFFICULTY1_NESTCOLUMN;
			numberOfTiles = Constants::DIFFICULTY1_SIDE_SIZE * Constants::DIFFICULTY1_SIDE_SIZE;
		}

		else if (_difficulty == 2) {
			parameters_maze.nbColumn = Constants::DIFFICULTY2_SIDE_SIZE;
			parameters_maze.nbLine = Constants::DIFFICULTY2_SIDE_SIZE;
			parameters_maze.nbFood = Constants::DIFFICULTY2_NBFOOD;
			parameters_maze.nestLine = Constants::DIFFICULTY2_NESTLINE;
			parameters_maze.nestColumn = Constants::DIFFICULTY2_NESTCOLUMN;
			numberOfTiles = Constants::DIFFICULTY2_SIDE_SIZE * Constants::DIFFICULTY2_SIDE_SIZE;
		}

		else if (_difficulty == 3) {
			parameters_maze.nbColumn = Constants::DIFFICULTY3_SIDE_SIZE;
			parameters_maze.nbLine = Constants::DIFFICULTY3_SIDE_SIZE;
			parameters_maze.nbFood = Constants::DIFFICULTY3_NBFOOD;
			parameters_maze.nestLine = Constants::DIFFICULTY3_NESTLINE;
			parameters_maze.nestColumn = Constants::DIFFICULTY3_NESTCOLUMN;
			numberOfTiles = Constants::DIFFICULTY3_SIDE_SIZE * Constants::DIFFICULTY3_SIDE_SIZE;
		}

		parameters_maze.difficulty = _difficulty;

		// Generate the maze of the game
		p_Maze = generateMaze(&parameters_maze);

		// Create the pheromons vector
		std::vector<float> vector1(numberOfTiles, 0.0);
		p_pheromons = std::move(vector1);

		// Set the number of actual players to 0
		p_actual_players = 0;

	}

	game::~game()
	{
		freeMaze(&p_Maze);
	}


	void game::join(const boost::uuids::uuid& _player_uuid, std::shared_ptr<session> _session)
	{
		// Create a player to add to the game
		Server::Player player_to_add;
		player_to_add.actual_line = p_Maze->nestLine;
		player_to_add.actual_column = p_Maze->nestColumn;
		player_to_add.has_food = false;
		player_to_add.p_uuid = _player_uuid;
		player_to_add._session = _session;

		// Add the player
		p_players.push_back(player_to_add);
		p_actual_players += 1;

		// Send the maze to the player 
		_session->setGame(this);
		_session->sendMaze(_player_uuid, p_Maze);
	}

	void game::move(const boost::uuids::uuid& _player, const std::string& _move)
	{
		Server::Player* player = nullptr;
		
		// Find the player with the given UUID	
		
		for (auto& p : p_players) {
			if (p.p_uuid == _player) {
				player = &p;
			}
		}

		if (!player) throw "Somehow, we didn't find the player that did the move...";

		std::cout << "Player infos : " << std::endl;
		std::cout << "----- Has food ? " << player->has_food << std::endl;

		// Deals with the movement given
		if (_move == "haut") {
			(player->actual_line) -= 1;

		}

		if (_move == "bas") {
			(player->actual_line) += 1;
		}

		if (_move == "gauche") {
			(player->actual_column) -= 1;
		}

		if (_move == "droite") {
			(player->actual_column) += 1;
		}

		/* If the value of hte tile where the player is is between 16 and 32, the player is actually on a food source
			We change the value of hasFood to TRUE
		*/
		

		auto hasFood = [&](const uint8_t tile) {return (tile & 16) == 16; };
		auto isNest = [&](const uint8_t tile) {return (tile & 32); };
		auto getPlayerTile = [&]() {

			return  p_Maze->tiles[player->actual_line * p_Maze->nbColumn + player->actual_column];
		};
		auto getPlayerTileIndice = [&]() {
			return player->actual_line * p_Maze->nbColumn + player->actual_column;
		};
		uint8_t actualPosition = getPlayerTile();
		std::cout << "Server side position : " << player->actual_line << " :" << player->actual_column << std::endl;
		if (hasFood(actualPosition) && player->has_food == false) {
			player->has_food = true;
			std::cout << "player : " << boost::uuids::to_string(player->p_uuid) << " has found food ! " << std::endl;
		}

		// If the player arrrive at Nest with food, he give it to the nest so doesn't has it anymore

		if (isNest(actualPosition) && player->has_food == true) {
			player->has_food = false;
		}

		/* If the player has food at this moment of code, this means he's on a normal tile, we just increase the pheromons value of this tile */

		if (player->has_food == true) {
			p_pheromons[getPlayerTileIndice()] += Constants::PHEROMON_DROP_AMOUNT;
		}


	}

	void game::remove(std::shared_ptr<session> _session)
	{
		int i = 0;

		while (p_players[i]._session != _session) {
			i++;
		}

		p_players.erase(p_players.begin() + i);
		p_actual_players -= 1;

		// We stop the game if there is no players playing
		if (p_actual_players == 0) {
			endGame();
		}
	}


	void game::endGame() {

		delete this;
	}


	void game::decreasePheromons()
	{
		/*
		- vector[i] <- (1 - evaporation) * vector[i]
		- players[i].session.sendInfo(vector)
		*/

		// Decreasing the value of each tile
		for (int i = 0; i < numberOfTiles; i++) {
			p_pheromons[i] = (1 - Constants::PHEROMON_DECREASE_AMOUNT) * p_pheromons[i];
		}

		// Sending the new value to each player

		for (int j = 0; j < p_actual_players; j++) {
			p_players[j]._session->sendPheromons(p_players[j].p_uuid, p_pheromons);
		}
	}
