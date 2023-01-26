#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.inl>
#include <vector>

#include "marble/abstraction/UnifiedRenderer.h"
#include <glad/glad.h>

static std::string vs = R"glsl(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec3 aColor;

			out vec2 v_TexCoord;
			out vec4 v_Color;

			//uniform mat2 u_R;
			uniform mat4 u_VP;

			void main(){
	
				vec4 vertex = vec4(position, 1);
				//vertex.xz = u_R * vertex.xz;
				gl_Position = u_VP * vertex;
				
				v_Color = vec4(aColor,1.F);
			};
		)glsl";

static std::string fs = R"glsl(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			};
		)glsl";



class PheremonesManager
{

private:

	std::vector<float> m_pheromones;
	

	struct QuadVertex {

		glm::vec3 position;
		glm::vec3 color;

	};

	struct BatchRenderer {

		Renderer::Shader shader{ vs, fs };

		Renderer::VertexArray m_VAO;
		Renderer::VertexBufferObject m_VBO;
		Renderer::IndexBufferObject m_IBO;

		QuadVertex* m_buffer = nullptr;
		QuadVertex* m_bufferPtr = nullptr;
		unsigned int m_indexCount = 0;

	} m_renderer ;

	size_t MaxQuadCount;
	size_t MaxVertexCount;
	size_t MaxIndicesCount;
	size_t MaxTextures;

public:
	struct MazeProperties {

		glm::uvec2 mazeDimensions;
		glm::vec3 startingPoint;
		float wallSize;
		float corridorSpace;

	} m_mazeProps;


public:

	PheremonesManager(const MazeProperties& props) 
	{
		m_mazeProps = props;

		for (int i = 0; i < props.mazeDimensions.x * props.mazeDimensions.y; i++) {
			m_pheromones.push_back((float)rand()/(float)RAND_MAX);
		}
		

		// --------- Batch renderer initialisation

		MaxQuadCount = props.mazeDimensions.x * props.mazeDimensions.y; // capped at number of tiles, might be an evolution point
		MaxVertexCount = MaxQuadCount * 4;
		MaxIndicesCount = MaxQuadCount * 6;
		MaxTextures = 32;


		m_renderer.m_buffer = new QuadVertex[MaxVertexCount];
		unsigned int* indices = new unsigned int[MaxIndicesCount];
		unsigned int offset = 0;

		for (int i = 0; i < MaxIndicesCount; i += 6) {

			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		m_renderer.m_VAO = Renderer::VertexArray();
		m_renderer.m_VBO = Renderer::VertexBufferObject(MaxVertexCount * sizeof(QuadVertex));

		Renderer::VertexBufferLayout layout;
		layout.push<float>(3);		// Position
		layout.push<float>(3);		// Color



		m_renderer.m_IBO = Renderer::IndexBufferObject(indices, MaxIndicesCount);


		m_renderer.m_VAO.addBuffer(m_renderer.m_VBO, layout, m_renderer.m_IBO);



	}

	~PheremonesManager() {
		delete[] m_renderer.m_buffer;
	}

	void setPheromones(const std::vector<float>& val) {
		m_pheromones = val;
	}

	void step(float delta, const glm::vec3& lookAt) {

		m_renderer.m_bufferPtr = m_renderer.m_buffer;
		prepareRendering(delta, lookAt);

		GLsizeiptr size = (uint8_t*)m_renderer.m_bufferPtr - (uint8_t*)m_renderer.m_buffer;

		m_renderer.m_VBO.bind();
		m_renderer.m_VAO.sendToGPU(size, m_renderer.m_buffer);
	}

	void render(const Renderer::Camera& camera) 
	{

		/*
		float theta = 3.14f - camera.getYaw();
		float c = cos(theta), s = sin(theta);
		glm::mat2 facingCameraRotationMatrix = glm::mat2(c, s, -s, c);
		*/




		m_renderer.shader.bind();
		m_renderer.shader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
		//m_renderer.shader.setUniformMat2f("u_R", facingCameraRotationMatrix);

		glDisable(GL_CULL_FACE);

		m_renderer.m_VAO.bind();
		m_renderer.m_IBO.bind();
		glDrawElements(GL_TRIANGLES, m_renderer.m_indexCount, GL_UNSIGNED_INT, nullptr);

		m_renderer.m_indexCount = 0;
		m_renderer.m_VAO.unbind();


	}

	
private:

		void prepareRendering(float delta, const glm::vec3& playerPosition)
		{


			for (unsigned int y = 0; y < m_mazeProps.mazeDimensions.y; y++) {

				for (unsigned int x = 0; x < m_mazeProps.mazeDimensions.x; x++) {

					int index = y * m_mazeProps.mazeDimensions.x + x;
					float value = m_pheromones[index];
					glm::vec3 position = computePheromonePosition({ x,y });


					drawPheromone(position, {value, value, value}, {2,2,2}, playerPosition);

				}
			}

		}

		inline glm::vec3 computePheromonePosition(const glm::uvec2& tile) {

			float x = (0.5 + tile.x) * (m_mazeProps.corridorSpace + m_mazeProps.wallSize);
			float y = 3;
			float z = (0.5 + tile.y) * (m_mazeProps.corridorSpace + m_mazeProps.wallSize);

			glm::vec3 res = m_mazeProps.startingPoint + glm::vec3{x, y, z};

			return res;
		}

		inline void drawPheromone(const glm::vec3& position, const glm::vec3& colors, const glm::vec3& size, const glm::vec3& playerPosition) {

			/*
			
			ALED
			*/
			glm::vec3 dir = glm::normalize(playerPosition - position);
			glm::vec3 v = glm::cross(dir, glm::vec3(0, 1, 0));
			glm::vec3 u = glm::cross(v, dir);

			glm::mat4x3 v_positions = {
				{ position.x, position.y, position.z },
				{ position.x + size.x, position.y,  position.z },
				{ position.x + size.x, position.y + size.y, position.z },
				{ position.x , position.y + size.y,  position.z }
			};

			glm::mat3 P = {
				dir,
				u,
				v
			};

			v_positions = P *  v_positions;

			// Bottom left
			m_renderer.m_bufferPtr->position = v_positions[0];// 2D only
			m_renderer.m_bufferPtr->color = colors;
			m_renderer.m_bufferPtr++;

			// Bottom right
			m_renderer.m_bufferPtr->position = v_positions[1]; // 2D only
			m_renderer.m_bufferPtr->color = colors;
			m_renderer.m_bufferPtr++;

			//top right
			m_renderer.m_bufferPtr->position = v_positions[2]; // 2D only
			m_renderer.m_bufferPtr->color = colors;
			m_renderer.m_bufferPtr++;

			// top left
			m_renderer.m_bufferPtr->position = v_positions[3]; // 2D only
			m_renderer.m_bufferPtr->color = colors;
			m_renderer.m_bufferPtr++;

			m_renderer.m_indexCount += 6;

		}



};

