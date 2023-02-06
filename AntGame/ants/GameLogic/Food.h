#pragma once

#include "AntsPlayer.h"
#include <glm/vec2.hpp>
#include "../Maze/MazeTileSystem.h"

constexpr float PICKUP_FOOD_RATE = 0.05f;

class FoodLogic
{

private:

	glm::uvec2 m_NestPosition;
	std::vector<glm::uvec2> m_FoodSourcesTiles;
	std::vector<float> m_quantities;
	bool m_holdsFood;
	
public:

	FoodLogic(const Maze& maze) : m_holdsFood(false) {

		/* TODO : this is close to duplicate code from FoodMeshGenerator.h */
		for (unsigned int y = 0; y < maze.nbLine; y++) {

			for (unsigned int x = 0; x < maze.nbColumn; x++) {

				int i = y * maze.nbColumn + x;
				uint8_t tile = maze.tiles[i];
				bool isFoodSpot = ((tile & 16) == 16);
				m_FoodSourcesTiles.push_back({ x,y });
				m_quantities.push_back(1.F);

			}
		}

		m_NestPosition = { maze.nestColumn,maze.nestLine };

	}

	void step(const glm::uvec2& playerPosition) {

		for (unsigned int i = 0; i < m_FoodSourcesTiles.size(); i++) {
			if (m_FoodSourcesTiles[i] == playerPosition) {

				m_holdsFood = true;
				std::cout << "walked on food source !" << std::endl;
				(m_quantities[i] > 0.05) ? m_quantities[i] -= PICKUP_FOOD_RATE : 0/*todo: do somthing when the food is empty ?*/;
				break;
			}
		}

		if (playerPosition == m_NestPosition) {
			m_holdsFood = false;
			/* increase the score*/
		}
	}

	bool doesPlayerCurrentlyHoldFood() const {
		return m_holdsFood;
	}

	std::vector<glm::uvec2> getFoodSourcesTiles() const {
		m_FoodSourcesTiles;
	}

	std::vector<float> getFoodSourcesQuantities() const {
		return m_quantities;
	}



};

