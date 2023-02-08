#pragma once

#include <glm/glm.hpp>
#include <vector>


#include "Score.h"

#include "marble/abstraction/UnifiedRenderer.h"



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
				color.a = smoothstep(0.9,0.8, length(o_uv*2-1.f));
				vec3 dis = texture(u_dudvMap, tmp).rgb;

				color.rgb = mix(vec3(dis.r/2.F),v_Color.rgb,0.7);
				color.rgb *= 2.f;


			};
		)glsl";


class PheremonesManager
{


public:

	struct MazeProperties {

		glm::uvec2 mazeDimensions;
		glm::vec3 startingPoint;
		float wallSize;
		float corridorSpace;
		Maze maze; /*To know nest and food locations*/

	} m_mazeProps;

	ScoreManager* scoreSystem = nullptr;


public:

	PheremonesManager(const MazeProperties& props);

	~PheremonesManager();

	inline void setPheromones(const std::vector<float>& val) {
		m_pheromones = val;
	}

	void step(float delta, const glm::vec3& lookAt);

	void render(const Renderer::Camera& camera);


private:

	void prepareRendering(float delta, const glm::vec3& playerPosition);

	inline glm::vec3 computePheromonePosition(const glm::uvec2& tile);
	inline void drawPheromone(const glm::vec3& position, const glm::vec3& colors, const glm::vec3& size, const glm::vec3& playerPosition);
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
};

