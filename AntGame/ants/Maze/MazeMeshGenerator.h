#pragma once

#include "marble/abstraction/Mesh.h"



#include <glm/vec2.hpp>
#include <array>

class MazeMeshGenerator
{
private:



	/*
	*      ______________
	*     |              |
	*     |				 |
	*     |				 |
	*     |				 |
	*     |				 |
	*     |				 |
	*     |______________|
	*/

	struct PillarNode {

		glm::uvec2 position;
		std::array<Renderer::Vertex, 8> m_vertices;

		PillarNode(const glm::vec2& p) {
			position = p;
			// compute verticies
		}

	};

public:

	static Renderer::Mesh generateMazeMesh(unsigned int width, unsigned int height);
	

};

