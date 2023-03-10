#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include "marble/abstraction/Mesh.h"
#include "marble/abstraction/UnifiedRenderer.h"

#include "MazeGeneration_proj/libAntMaze.h"

#include "MazeMeshGenerator.h" // todo move constants somewhere

#include "marble/World/Props/Prop.h"

namespace MazeMeshGenerator
{

	class Nest : public World::Prop 
	{
		std::shared_ptr<Renderer::Mesh> m_model = std::make_shared<Renderer::Mesh>(Renderer::loadMeshFromFile("res/meshes/nest.obj"));
	public:
		Nest() : World::Prop{m_model} {}
	};

	class FoodSource : public World::Prop 
	{
	
		std::shared_ptr<Renderer::Mesh> m_model = std::make_shared<Renderer::Mesh>(Renderer::loadMeshFromFile("res/meshes/cube.obj"));
	
	public:
	
		
		FoodSource() 
			: World::Prop{ m_model }
		{
			
			static int foodCount = 0;
			foodCount++;
			name = "FoodSource#" + foodCount;
			
		}
	};

	/*
	* Feed this vector into a props manager ! 
	*/
	std::vector<World::Prop> generateFoodSources(const Maze& maze, const glm::vec3& mazeStartingPoint=glm::vec3{0,0,0})
	{

		static std::shared_ptr<Renderer::Mesh> model = std::make_shared<Renderer::Mesh>(Renderer::loadMeshFromFile("res/meshes/croissant_01_L0.obj"));


		std::vector<World::Prop> res;
		uint8_t* tiles = maze.tiles;



		auto computeFoodPosition = [&](const glm::uvec2 tile) {

			float x = (0.5f + tile.x) * (CORRIDOR_SPACE);
			float y = 3; // TODO find the correct height
			float z = (0.5f + tile.y) * (CORRIDOR_SPACE);

			glm::vec3 res = mazeStartingPoint + glm::vec3{ x, y, z };

			return res;
		};


		for (unsigned int y = 0; y < maze.nbLine; y++) {

			for (unsigned int x = 0; x < maze.nbColumn; x++) {


				int i = y * maze.nbColumn + x;
				uint8_t tile = tiles[i];
				bool isFoodSpot = ((tile & HAS_FOOD) == HAS_FOOD);
				if (isFoodSpot) { 

					glm::vec3 position = computeFoodPosition({ x, y });
					res.emplace_back(World::Prop{ model, position });
				}
			}
		}
		return res;

	}

	World::Prop generateNest(const Maze& maze, const glm::vec3& mazeStartingPoint = glm::vec3{ 0,0,0 }) {
		// todo rework this, this is code duplication of above function

		std::shared_ptr<Renderer::Mesh> model = std::make_shared<Renderer::Mesh>(Renderer::loadMeshFromFile("res/meshes/fattree.obj"));


		World::Prop res;
		uint8_t* tiles = maze.tiles;



		auto computeNestPosition = [&](const glm::uvec2 tile) {

			float x = (0.5f + tile.x) * (CORRIDOR_SPACE);
			float y = 0.5; // TODO find the correct height
			float z = (0.5f + tile.y) * (CORRIDOR_SPACE);

			glm::vec3 res = mazeStartingPoint + glm::vec3{ x, y, z };

			return res;
		};


		for (unsigned int y = 0; y < maze.nbLine; y++) {

			for (unsigned int x = 0; x < maze.nbColumn; x++) {


				int i = y * maze.nbColumn + x;
				uint8_t tile = tiles[i];
				bool isNest = ((tile & HAS_NEST) == HAS_NEST);
				if (isNest) {

					glm::vec3 position = computeNestPosition({ x, y });
					res = World::Prop{ model, position };
					res.size = { 0.5,0.5,0.5 };
					return res;
				}
			}
		}
		return res;



	}




}