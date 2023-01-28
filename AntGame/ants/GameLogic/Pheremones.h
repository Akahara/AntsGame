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
			layout(location = 2) in vec2 i_uv;

			out vec2 v_TexCoord;
			out vec4 v_Color;
			out vec2 o_uv;
			out vec4 o_pos;

			uniform mat4 u_VP;
			uniform float u_time;

			void main(){
	
				vec4 vertex = vec4(position, 1);
				vertex.y += cos(u_time);
				o_uv = i_uv;
				v_Color = vec4(aColor,1.F);
				o_pos =  u_VP * vertex;
				gl_Position = o_pos;
			};
		)glsl";

static std::string fs = R"glsl(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_Color;
			in vec2 o_uv;
			in vec4 o_pos;
		
			uniform sampler2D u_texture;
			uniform sampler2D u_dudvMap;
			uniform sampler2D u_normal;
			uniform float u_time;
			uniform float u_moveFactor;

			vec3 sunPos = vec3(100,100,100);

			void main()
			{

				vec2 screenspace = ((o_pos.xy/o_pos.w)+1) * 0.5;

				vec2 tmp = vec2(screenspace.x,  screenspace.y);

				vec2 distortedTexCoords = texture(u_dudvMap, vec2(o_uv.x + u_moveFactor, o_uv.y)).rg*0.1;
				distortedTexCoords = o_uv + vec2(distortedTexCoords.x, distortedTexCoords.y+u_moveFactor);
				vec2 distortion = (texture(u_dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * 0.04f;

				tmp += distortion;


	

				color.a = texture(u_texture, o_uv).a;
				vec3 dis = texture(u_dudvMap, tmp).rgb;

				color.rgb = mix(dis,v_Color.rgb,0.7);
				color.rgb *= 4.f;


			};
		)glsl";



class PheremonesManager
{

private:

	std::vector<float> m_pheromones;

	Renderer::Texture m_texture = Renderer::Texture("res/textures/pheromones.png");
	Renderer::Texture m_distortion = Renderer::Texture("res/textures/dudvWater.png");
	Renderer::Texture m_normal = Renderer::Texture("res/textures/waterNormal.png");
	float m_realtime;
	float m_moveFactor = 0;

	struct QuadVertex {

		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;

	};

	struct BatchRenderer {

		Renderer::Shader shader{ vs, fs };

		Renderer::VertexArray m_VAO;
		Renderer::VertexBufferObject m_VBO;
		Renderer::IndexBufferObject m_IBO;

		QuadVertex* m_buffer = nullptr;
		QuadVertex* m_bufferPtr = nullptr;
		unsigned int m_indexCount = 0;

	} m_renderer;

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
			m_pheromones.push_back((float)rand() / (float)RAND_MAX);
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
		layout.push<float>(2);		// uv



		m_renderer.m_IBO = Renderer::IndexBufferObject(indices, MaxIndicesCount);
		m_renderer.m_VAO.addBuffer(m_renderer.m_VBO, layout, m_renderer.m_IBO);

		m_renderer.shader.bind();
		m_renderer.shader.setUniform1i("u_texture", 0);
		m_renderer.shader.setUniform1i("u_dudvMap", 1);
		m_renderer.shader.setUniform1i("u_normal", 2);
		m_renderer.shader.unbind();



	}

	~PheremonesManager() {
		delete[] m_renderer.m_buffer;
	}

	void setPheromones(const std::vector<float>& val) {
		m_pheromones = val;
	}

	void step(float delta, const glm::vec3& lookAt) {

		m_realtime += delta;
		m_renderer.shader.bind();
		m_renderer.shader.setUniform1f("u_time", m_realtime);

		m_moveFactor += 0.15 * delta;
		m_moveFactor = std::fmod(m_moveFactor, 1.f);
		m_renderer.shader.bind();
		m_renderer.shader.setUniform1f("u_moveFactor", m_moveFactor);


		m_renderer.shader.unbind();

		m_renderer.m_bufferPtr = m_renderer.m_buffer;
		prepareRendering(delta, lookAt);

		GLsizeiptr size = (uint8_t*)m_renderer.m_bufferPtr - (uint8_t*)m_renderer.m_buffer;

		m_renderer.m_VBO.bind();
		m_renderer.m_VAO.sendToGPU(size, m_renderer.m_buffer);
	}

	void render(const Renderer::Camera& camera)
	{


		m_renderer.shader.bind();
		m_renderer.shader.setUniformMat4f("u_VP", camera.getViewProjectionMatrix());
		m_texture.bind(0);
		m_distortion.bind(1);
		m_normal.bind(2);

		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		m_renderer.m_VAO.bind();
		m_renderer.m_IBO.bind();
		glDrawElements(GL_TRIANGLES, m_renderer.m_indexCount, GL_UNSIGNED_INT, nullptr);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

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


				drawPheromone(position, glm::lerp({1,0,0.6}, glm::vec3{0,0,0}, value), glm::lerp({ 4,4,4 }, glm::vec3{1,1,1}, value), playerPosition);

			}
		}

	}

	inline glm::vec3 computePheromonePosition(const glm::uvec2& tile) {

		float x = (0.5 + tile.x) * (m_mazeProps.corridorSpace);
		float y = 3;
		float z = (0.5 + tile.y) * (m_mazeProps.corridorSpace);

		glm::vec3 res = m_mazeProps.startingPoint + glm::vec3{ x, y, z };

		return res;
	}

	inline void drawPheromone(const glm::vec3& position, const glm::vec3& colors, const glm::vec3& size, const glm::vec3& playerPosition) {
		glm::vec3 dir = glm::normalize(playerPosition - position);
		glm::vec3 v = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
		glm::vec3 u = glm::normalize(glm::cross(v, dir));

		// the model is a plane facing -z, centered on the origin
		glm::vec3 v_positions[4]{
			{ -size.x * .5f, -size.y * .5f, 0 },
			{ +size.x * .5f, -size.y * .5f, 0 },
			{ +size.x * .5f, +size.y * .5f, 0 },
			{ -size.x * .5f, +size.y * .5f, 0 },
		};

		glm::vec2 v_uv[4]{
			{ 0,0 },
			{ 1,0 },
			{ 1,1 },
			{ 0,1 },
		};

		// rotate it so that it faces the camera
		glm::mat3 P = {
			u,
			v,
			dir,
		};

		// and also add its world position (this is basically the M matrix precomputed)
		v_positions[0] = P * v_positions[0] + position;
		v_positions[1] = P * v_positions[1] + position;
		v_positions[2] = P * v_positions[2] + position;
		v_positions[3] = P * v_positions[3] + position;

		// Bottom left
		m_renderer.m_bufferPtr->position = v_positions[0];// 2D only
		m_renderer.m_bufferPtr->color = colors;
		m_renderer.m_bufferPtr->uv = v_uv[0];
		m_renderer.m_bufferPtr++;

		// Bottom right
		m_renderer.m_bufferPtr->position = v_positions[1]; // 2D only
		m_renderer.m_bufferPtr->color = colors;
		m_renderer.m_bufferPtr->uv = v_uv[1];
		m_renderer.m_bufferPtr++;

		//top right
		m_renderer.m_bufferPtr->position = v_positions[2]; // 2D only
		m_renderer.m_bufferPtr->color = colors;
		m_renderer.m_bufferPtr->uv = v_uv[2];
		m_renderer.m_bufferPtr++;

		// top left
		m_renderer.m_bufferPtr->position = v_positions[3]; // 2D only
		m_renderer.m_bufferPtr->color = colors;
		m_renderer.m_bufferPtr->uv = v_uv[3];
		m_renderer.m_bufferPtr++;

		m_renderer.m_indexCount += 6;

	}



};

