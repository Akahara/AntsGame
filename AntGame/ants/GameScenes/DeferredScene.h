#pragma once


#include "marble/abstraction/Window.h"
#include "marble/abstraction/UnifiedRenderer.h"
#include "marble/abstraction/DeferredRenderer.h"
#include "Scene.h"
#include "marble/World/Player.h"

class DeferredScene : public Scene {
private:

	Renderer::Mesh m_cube = Renderer::createCubeMesh();
	Renderer::Mesh m_plane = Renderer::createPlaneMesh();
	Renderer::Mesh m_sphere = Renderer::createSphereMesh(20);
	Player m_player;
	World::Sky m_sky;

	DeferredRenderer m_deferredRenderer;

	float m_realtime = 0;

public:
	DeferredScene()
	{
		{
			int samplers[8] = { 0,1,2,3,4,5,6,7 };
			Renderer::Shader& meshShader = Renderer::rebuildStandardMeshShader(Renderer::ShaderFactory()
				.prefix("res/shaders/")
				.addFileVertex("standard.vs")
				.prefix("mesh_parts/")
				.addFileFragment("base.fs")
				.addFileFragment("color_singletexture.fs")
				.addFileFragment("lights_pointlights.fs")
				.addFileFragment("final_fog.fs")
				.addFileFragment("shadows_normal.fs")
				.addFileFragment("normal_normalmap.fs"));
			meshShader.bind();
			meshShader.setUniform1f("u_Strength", 1.25f);
			meshShader.setUniform1iv("u_Textures2D", 8, samplers);
			meshShader.setUniform3f("u_fogDamping", .002f, .002f, .004f);
			meshShader.setUniform3f("u_fogColor", 1.000f, 0.944f, 0.102f);
			meshShader.setUniform3f("u_SunPos", 1000, 1000, 1000);
			Renderer::Shader::unbind();
		}
	
		m_plane.bindTextureToSlot(std::make_shared<Renderer::Texture>("res/textures/sand.jpg"), 0);
	
	}

	void step(float delta) override
	{
		m_realtime += delta;
		m_player.step(delta);
	}

	void renderFn(Renderer::Camera& camera) 
	{
		Renderer::clear();
		Renderer::renderMesh(camera, { 5,0,5 }, { 2,2,2 }, m_cube);
		Renderer::renderMesh(camera, { 0,-2,0 }, { 20,1,20 }, m_plane);
		Renderer::renderMesh(camera, { 0,0,0 }, { 2,2,2 }, m_sphere);
		m_sky.render(camera, m_realtime, false);

	}

	void onRender() override
	{
		m_deferredRenderer.render(
			// renderFn
			[&]() -> void
			{
				renderFn(m_player.getCamera());
			},
			// scene camera
			m_player.getCamera()
		);
	}

	void onImGuiRender() override 
	{
		m_deferredRenderer.renderImGuiDebugWindow();
	}



};

