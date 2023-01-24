#include "MazeMeshGenerator.h"

#include "MazeGeneration_proj/libAntMaze.h"

constexpr unsigned char NORTH = 1;
constexpr unsigned char EAST =	2;
constexpr unsigned char SOUTH = 4;
constexpr unsigned char WEST =	8;

/**
* 
* This method is probably subtoptimal right now, as we have to hold way more indices than need (5*8 for each pillar, 3*8 for each wall)
* TODO : optimize this, make it more flexible with the params too
*/
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


	std::vector<Renderer::Vertex> verticies;
	std::vector<unsigned int> indicies;

	constexpr float WALL_SIZE = 5.f;
	constexpr float WALL_HEIGHT = 30.F;
	constexpr float CORRIDOR_SPACE = 20.F;
	
	/*
	* float s3 = std::sqrtf(3);
  std::vector<Vertex> vertices{
    // position              uv            normal            // tex id          // color
    { { -.5f, -.5f, -.5f }, { 0.f, 0.f }, { -s3, -s3, -s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { +.5f, -.5f, -.5f }, { 1.f, 0.f }, { +s3, -s3, -s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { +.5f, +.5f, -.5f }, { 1.f, 1.f }, { +s3, +s3, -s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { -.5f, +.5f, -.5f }, { 0.f, 1.f }, { -s3, +s3, -s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { -.5f, -.5f, +.5f }, { 0.f, 1.f }, { -s3, -s3, +s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { +.5f, -.5f, +.5f }, { 1.f, 1.f }, { +s3, -s3, +s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { +.5f, +.5f, +.5f }, { 1.f, 0.f }, { +s3, +s3, +s3 }, (float)texId, {1.0f, 1.0f, 0.0f}, },
    { { -.5f, +.5f, +.5f }, { 0.f, 0.f }, { -s3, +s3, +s3 }, (float)texId, {1.0f, 1.0f, 0.0f} },
	*/
	float s3 = std::sqrtf(3);
	// Generate a bunch of pillars
	for (unsigned int y = 0; y < height + 1; y++) {

		for (unsigned int x = 0; x < width + 1; x++) {

			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { -s3, -s3, -s3 } }); // base top_left
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { +s3, -s3, -s3 } }); // base top_right
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { +s3, -s3, +s3 } }); // base bot_right
			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, 0,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { -s3, -s3, +s3 } }); // base 

			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { -s3, +s3, -s3 } }); // top	top_left
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT, -WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { +s3, +s3, -s3 } }); // top  top_right
			verticies.emplace_back(Renderer::Vertex{ { WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { +s3, +s3, +s3 } }); // top  top_right
			verticies.emplace_back(Renderer::Vertex{ {-WALL_SIZE / 2.F + x * CORRIDOR_SPACE, WALL_HEIGHT,  WALL_SIZE / 2.F + y * CORRIDOR_SPACE}, {0,0}, { -s3, +s3, +s3 } }); // top  top_left

			// Fill the indicies for the pillar

			auto firstIndice = 8 * y * (width + 1) + 8 * x;

			// Back face
			indicies.push_back(firstIndice+1);
			indicies.push_back(firstIndice);
			indicies.push_back(firstIndice+5);
			indicies.push_back(firstIndice+5);
			indicies.push_back(firstIndice);
			indicies.push_back(firstIndice+4);

			// Left face
			indicies.push_back(firstIndice+4);
			indicies.push_back(firstIndice);
			indicies.push_back(firstIndice+7);
			indicies.push_back(firstIndice+3);
			indicies.push_back(firstIndice+7);
			indicies.push_back(firstIndice);

			// Right face
			indicies.push_back(firstIndice+5);
			indicies.push_back(firstIndice+6);
			indicies.push_back(firstIndice+1);
			indicies.push_back(firstIndice+6);
			indicies.push_back(firstIndice+2);
			indicies.push_back(firstIndice+1);

			// Front face
			indicies.push_back(firstIndice+3);
			indicies.push_back(firstIndice+2);
			indicies.push_back(firstIndice+6);
			indicies.push_back(firstIndice+7);
			indicies.push_back(firstIndice+3);
			indicies.push_back(firstIndice+6);

			// Top
			indicies.push_back(firstIndice+5);
			indicies.push_back(firstIndice+4);
			indicies.push_back(firstIndice+6);
			indicies.push_back(firstIndice+7);
			indicies.push_back(firstIndice+6);
			indicies.push_back(firstIndice+4);
			


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

		indicies.push_back(pillar_base_top_right + 8);
		indicies.push_back(pillar_base_top_right + 1);
		indicies.push_back(pillar_base_top_right + 5);

		indicies.push_back(pillar_base_top_right + 12);
		indicies.push_back(pillar_base_top_right + 8);
		indicies.push_back(pillar_base_top_right + 5);

		// -- South face

		indicies.push_back(pillar_base_top_right + 15);
		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 11);

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 11);

		// -- Top face, not necessary but why not

		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 15);
		indicies.push_back(pillar_base_top_right + 5);

		indicies.push_back(pillar_base_top_right + 5);
		indicies.push_back(pillar_base_top_right + 15);
		indicies.push_back(pillar_base_top_right + 12);


	};

	auto linkPillarVertical = [&](const glm::uvec2& pillar)
	{
		auto pillar_base_top_right = 8 * pillar.y * (width + 1) + 8 * pillar.x;
		unsigned int stride = (width+1) * 8;

		// -- Top face, not necessary but why not

		
		indicies.push_back(pillar_base_top_right + 7);
		indicies.push_back(pillar_base_top_right + 4 + stride);
		indicies.push_back(pillar_base_top_right + 6);

		indicies.push_back(pillar_base_top_right + 4 + stride);
		indicies.push_back(pillar_base_top_right + 5 + stride);
		indicies.push_back(pillar_base_top_right + 6);
		
		// -- east face


		indicies.push_back(pillar_base_top_right + 6);
		indicies.push_back(pillar_base_top_right + 5 + stride);
		indicies.push_back(pillar_base_top_right + 1 +stride);

		indicies.push_back(pillar_base_top_right + 1 + stride);
		indicies.push_back(pillar_base_top_right + 2);
		indicies.push_back(pillar_base_top_right + 6);

		// -- West face
		
		indicies.push_back(pillar_base_top_right + 4 + stride);
		indicies.push_back(pillar_base_top_right + 7);
		indicies.push_back(pillar_base_top_right + stride);

		indicies.push_back(pillar_base_top_right + 3);
		indicies.push_back(pillar_base_top_right + stride);
		indicies.push_back(pillar_base_top_right + 7);
	


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

		// -- Border checking
		if (x == 0) {
			linkPillarVertical({0, y});
		}

		if (y == 0) {
			linkPillarHorizontal({ x, 0});
		}

	}

	return Renderer::Mesh(verticies, indicies);


}