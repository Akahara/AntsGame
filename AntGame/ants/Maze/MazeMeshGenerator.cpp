#include "MazeMeshGenerator.h"

#include "MazeGeneration_proj/libAntMaze.h"

constexpr unsigned char NORTH = 1;
constexpr unsigned char EAST =	2;
constexpr unsigned char SOUTH = 4;
constexpr unsigned char WEST =	8;

Renderer::Mesh MazeMeshGenerator::generateMazeMesh(unsigned int width, unsigned int height) 

{

	ParamMaze params{
		width,
		height,
		0,
		0,
		1,
		2
	};

	Maze* maze = generateMazeCPP(&params);
	uint8_t* tiles = maze->tiles;

	// Generate a bunch of pillars (n+1)

	std::vector<Renderer::Vertex> verticies;
	std::vector<unsigned int> indicies;

	constexpr float WALL_SIZE = 5.f;
	constexpr float WALL_HEIGHT = 5.F;
	constexpr float CORRIDOR_SPACE = 20.F;

	/*
	
	struct Vertex {
	  glm::vec3 position;
	  glm::vec2 uv;
	  glm::vec3 normal;
	  glm::vec3 color = {1.0f, 0.f, 0.f};
	  float texId = 0;
	};
	*/
	

	for (unsigned int y = 0; y < height + 1; y++) {

		for (unsigned int x = 0; x < width + 1; x++) {

			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {1,0,0}, {1,0,1} }); // base top_left
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {1,0,0}, {1,1,0} }); // base top_right
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {1,0,0}, {1,0,0} }); // base bot_right
			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {1,0,0}, {1,1,1} }); // base 

			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {0,1,0},{0,0,1}  }); // top	top_left
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {0,1,0},{0,1,0}  }); // top  top_right
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {0,1,0},{0,0,0}  }); // top  top_right
			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, {0,1,0},{0,1,1}  }); // top  top_left

		}
	}

	std::cout << "stop" << std::endl;;

	/**
	* This lambda adds 6 triangles in the indicies vector.
	* It take a pillar position, and links it to the one to the right. 
	*/
	auto linkPillarHorizontal = [&](const glm::uvec2& pillar) 
	{
		auto pillar_base_top_right = 8 * pillar.y * (width + 1) + 8 * pillar.x;

		// -- North face

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 8);
		indicies.push_back(pillar_base_top_right + 1);

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 12);
		indicies.push_back(pillar_base_top_right + 8);

		// -- South face

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 15);
		indicies.push_back(pillar_base_top_right + 11);

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 11);

		// -- Top face, not necessary but why not

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 15);

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 12);
		indicies.push_back(pillar_base_top_right + 15);


	};

	auto linkPillarVertical = [&](const glm::uvec2& pillar)
	{
		auto pillar_base_top_right = 8 * pillar.y * (width + 1) + 8 * pillar.x;
		unsigned int stride = (width+1) * 8;

		// -- Top face, not necessary but why not

		indicies.push_back(pillar_base_top_right + 7);
		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 5 + stride);

		indicies.push_back(pillar_base_top_right + 4 + stride);
		indicies.push_back(pillar_base_top_right + 7);
		indicies.push_back(pillar_base_top_right + 6);

		// -- east face


		indicies.push_back(pillar_base_top_right + 3);
		indicies.push_back(pillar_base_top_right + stride);
		indicies.push_back(pillar_base_top_right + 4 + stride);

		indicies.push_back(pillar_base_top_right + 4);
		indicies.push_back(pillar_base_top_right + 7);
		indicies.push_back(pillar_base_top_right + 4 + stride);

		// -- West face

		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 1 + stride);
		indicies.push_back(pillar_base_top_right + 4 + stride);

		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 4 + stride);
		indicies.push_back(pillar_base_top_right + 6);


	};

	int tile_id = 0;
	while(tile_id < width * height){ // might be unsafe, change from pointer to vector

		uint8_t tile = *tiles;
		tiles++;
		int x = tile_id % width;
		int y = std::floor(tile_id / width);
		tile_id++;

		bool hasWallSouth = ((tile & SOUTH) == SOUTH);
		bool hasWallEast = ((tile & EAST) == EAST);

		if (hasWallSouth) {

			// Find the south west pillar of the tile
			glm::uvec2 pillarCoord{
				x,
				y + 1
			};

			// Link said pillar to next horizontaly

			linkPillarHorizontal(pillarCoord);

		}

		if (hasWallEast) {

			// Find the north east pillar of the tile
			glm::uvec2 pillarCoord{
				x+1,
				y
			};

			// Link said pillar to next horizontaly

			linkPillarVertical(pillarCoord);

		}



	}


	// -- Last step : west / north borders this doesnt work
	/*
	{

		auto pillar_base_top_right = 0;
		auto stride = (8 * width-1);

		// North face

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 8 + stride);
		indicies.push_back(pillar_base_top_right + 1);

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 8 + stride);
		indicies.push_back(pillar_base_top_right + 12 + stride);

		// -- South face

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 15 + stride);
		indicies.push_back(pillar_base_top_right + 11 + stride);

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 11 + stride);

		// -- Top face, not necessary but why not

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 15 + stride);

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 12 + stride);
		indicies.push_back(pillar_base_top_right + 15 + stride);
	}
	*/

	return Renderer::Mesh(verticies, indicies);


}