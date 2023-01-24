#pragma once

#include "marble/World/Player.h"
#include "marble/abstraction/UnifiedRenderer.h"

class AntsPlayer : public Player 
{

private:


	bool m_enableTPS;

	Renderer::Mesh m_mesh = Renderer::createCubeMesh();

	struct ThirdPersonParams{

		Renderer::Camera camera;
		glm::vec3 playerForward;
		glm::vec3 distanceFromSubject = {4,3,4};

	} m_tps;

public:

	AntsPlayer() : m_enableTPS(false) {}


	void step(float delta) override {
		Player::step(delta);

		m_tps.camera = m_camera;

		if (m_enableTPS) {

			glm::vec3 mesh_dir = m_camera.getForward();
			m_tps.playerForward = glm::normalize(glm::vec3{ mesh_dir.x, 0, mesh_dir.z });

			glm::vec3 new_position = m_camera.getPosition() ;
			m_tps.camera.moveCamera(-m_tps.playerForward * m_tps.distanceFromSubject + glm::vec3{0, m_tps.distanceFromSubject.y, 0});
			m_tps.camera.lookAt(m_camera.getPosition());
			m_tps.camera.recalculateViewMatrix();
			m_tps.camera.recalculateViewProjectionMatrix();

		}
	}

	void render() const {
		if (m_enableTPS) {

			Renderer::renderMesh(m_tps.camera, m_camera.getPosition(), { 1,1,1 }, m_mesh);

			Renderer::renderDebugLine(m_tps.camera, m_camera.getPosition(), m_camera.getPosition() + m_tps.playerForward);
			Renderer::renderDebugLine(m_tps.camera, m_camera.getPosition(), m_camera.getPosition() - m_tps.playerForward, { 1,0,0,1 });
		}
	}

	void switchViewMode() {
		m_enableTPS = !m_enableTPS;
	}

	Renderer::Camera& getCamera() override {
		return (m_enableTPS) ? m_tps.camera : m_camera;
	}

	void setTPSDistances(const glm::vec3& val) {
		m_tps.distanceFromSubject = val;

	}


};

