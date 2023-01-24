#pragma once

#include "marble/vendor/imgui/imgui.h"

#include "marble/World/Player.h"
#include "marble/abstraction/UnifiedRenderer.h"
#include "../Maze/MazeMeshGenerator.h"

class AntsPlayer : public Player 
{
private:
    Renderer::Mesh m_mesh = Renderer::createCubeMesh();


	bool m_enableTPS;

	Renderer::Mesh m_mesh = Renderer::createCubeMesh();
	glm::uvec2 m_tile;

    struct ThirdPersonParams {
        Renderer::Camera camera;
        glm::vec3 playerForward;
        glm::vec3 distanceFromSubject = { 5,1.4,5 };
        float cameraHeight = 2.f;
        const Maze *maze = nullptr; // not owned, used to avoid moving the camera into a wall
    } m_tps;

public:
    AntsPlayer() : m_tps{} {}

    void setMaze(const Maze &maze) { m_tps.maze = &maze; }
    Renderer::Camera &getCamera() override { return m_tps.camera; }
    void setTPSDistances(glm::vec2 dh) { m_tps.distanceFromSubject = dh.x; m_tps.cameraHeight = dh.y; }

    void step(float delta) override
    {
        Player::step(delta);

        m_tps.camera = m_camera;

        glm::vec3 meshDir = m_tps.camera.getForward();
        m_tps.playerForward = glm::normalize(glm::vec3{ meshDir.x, 0, meshDir.z });
        
        float cameraDistance = m_tps.distanceFromSubject;
        if (m_tps.maze != nullptr) {
          glm::vec2 rayOrigin = { m_camera.getPosition().x, m_camera.getPosition().z };
          glm::vec2 rayDirection = { -m_tps.playerForward.x, -m_tps.playerForward.z };
          cameraDistance = glm::min(cameraDistance, raycastMaze(rayOrigin, rayDirection, 30.f));
        }
        m_tps.camera.moveCamera(-m_tps.playerForward * cameraDistance + glm::vec3{0, m_tps.cameraHeight, 0});
        m_tps.camera.lookAt(getPosition());
        m_tps.camera.recalculateViewMatrix();
        m_tps.camera.recalculateViewProjectionMatrix();
    }

    void render(const Renderer::Camera &viewCamera) const
    {
        Renderer::renderMesh(viewCamera, getPosition(), { 1,1,1 }, m_mesh);

        Renderer::renderDebugLine(viewCamera, getPosition(), getPosition() + m_tps.playerForward);
        Renderer::renderDebugLine(viewCamera, getPosition(), getPosition() - m_tps.playerForward, { 1,0,0,1 });
        Renderer::renderDebugLine(viewCamera, getPosition(), m_tps.camera.getPosition(), { 1,1,0,1 });

        debugRaycast(viewCamera);
    }

    float raycastMaze(glm::vec2 rayOrigin, glm::vec2 rayDirection, float maxDistance) const
    {
      // avoid math exceptions by subterfuging the input values (not noticeable)
      if (rayDirection.x == 0) rayDirection.x = .01f;
      if (rayDirection.y == 0) rayDirection.y = .01f;

      constexpr float cellSize = MazeMeshGenerator::CORRIDOR_SPACE;
      constexpr float wallSize = MazeMeshGenerator::WALL_SIZE;

      float hitDistanceSquared = maxDistance * maxDistance;

      // raycast toward "x=constant" lines of the grid
      for (int i = 0; ; i++) {
        float nextX = glm::floor(rayOrigin.x / cellSize + i * glm::sign(rayDirection.x)) * cellSize + (rayDirection.x > 0 ? cellSize - wallSize * .5f : wallSize * .5f);
        float yAtNextX = rayOrigin.y + (nextX - rayOrigin.x) / rayDirection.x * rayDirection.y;

        float distanceSquared = (nextX - rayOrigin.x) * (nextX - rayOrigin.x) + (yAtNextX - rayOrigin.y) * (yAtNextX - rayOrigin.y);
        if (distanceSquared >= hitDistanceSquared)
          break;

        int mazeX = (int)(nextX / cellSize);
        int mazeY = (int)(yAtNextX / cellSize);
        if (
          // hit a corner
          glm::mod(yAtNextX + wallSize * .5f, cellSize) < wallSize ||
          // hit a wall
          (mazeX >= 0 && (uint32_t)mazeX < m_tps.maze->nbLine && mazeY >= 0 && (uint32_t)mazeY < m_tps.maze->nbColumn &&
          (m_tps.maze->tiles[mazeX + mazeY * m_tps.maze->nbColumn] & (rayDirection.x < 0 ? MazeMeshGenerator::WEST : MazeMeshGenerator::EAST)))
        ) {
          hitDistanceSquared = distanceSquared;
          break;
        }
      }

      // raycast toward "y=constant" lines of the grid
      for (int i = 0; ; i++) {
        float nextY = glm::floor(rayOrigin.y / cellSize + i * glm::sign(rayDirection.y)) * cellSize + (rayDirection.y > 0 ? cellSize - wallSize * .5f : wallSize * .5f);
        float xAtNextY = rayOrigin.x + (nextY - rayOrigin.y) / rayDirection.y * rayDirection.x;

        float distanceSquared = (xAtNextY - rayOrigin.x) * (xAtNextY - rayOrigin.x) + (nextY - rayOrigin.y) * (nextY - rayOrigin.y);
        if (distanceSquared >= hitDistanceSquared)
          break;

        int mazeY = (int)(nextY / cellSize);
        int mazeX = (int)(xAtNextY / cellSize);
        if (
          // hit a corner
          glm::mod(xAtNextY + wallSize * .5f, cellSize) < wallSize ||
          // hit a wall
          (mazeX >= 0 && (uint32_t)mazeX < m_tps.maze->nbLine && mazeY >= 0 && (uint32_t)mazeY < m_tps.maze->nbColumn &&
          (m_tps.maze->tiles[mazeX + mazeY * m_tps.maze->nbColumn] & (rayDirection.y < 0 ? MazeMeshGenerator::NORTH : MazeMeshGenerator::SOUTH)))
          ) {
          hitDistanceSquared = distanceSquared;
          break;
        }
      }

      return glm::sqrt(hitDistanceSquared);
    }

    void debugRaycast(const Renderer::Camera &camera) const // TODO remove completely, its just nice to have comited somewhere
    {
      glm::vec2 rayOrigin = { m_camera.getPosition().x, m_camera.getPosition().z };
      glm::vec2 rayDirection = { -m_tps.playerForward.x, -m_tps.playerForward.z };
      float maxDistance = 300.f;
      if (rayDirection.x == 0)
        rayDirection.x = .01f;

      constexpr float cellSize = MazeMeshGenerator::CORRIDOR_SPACE;
      constexpr float wallSize = MazeMeshGenerator::WALL_SIZE;
      
      float hitDistanceSquared = maxDistance * maxDistance;

      for (int i = 0; ; i++) {
        float nextX = glm::floor(rayOrigin.x / cellSize+i*glm::sign(rayDirection.x)) * cellSize + (rayDirection.x > 0 ? cellSize - wallSize * .5f : wallSize * .5f);
        float yAtNextX = rayOrigin.y + (nextX - rayOrigin.x) / rayDirection.x * rayDirection.y;
        
        if((nextX-rayOrigin.x)*(nextX-rayOrigin.x) + (yAtNextX-rayOrigin.y)*(yAtNextX-rayOrigin.y) >= hitDistanceSquared)
          break;

        bool intersection = false;

        int mazeX = (int)(nextX / cellSize);
        int mazeY = (int)(yAtNextX / cellSize);
        if (
          glm::mod(yAtNextX + wallSize * .5f, cellSize) < wallSize ||
          (mazeX >= 0 && (uint32_t)mazeX < m_tps.maze->nbLine && mazeY >= 0 && (uint32_t)mazeY < m_tps.maze->nbColumn &&
          (m_tps.maze->tiles[mazeX + mazeY * m_tps.maze->nbColumn] & (rayDirection.x < 0 ? MazeMeshGenerator::WEST : MazeMeshGenerator::EAST)))
          ) {
          intersection = true;
        }
        Renderer::renderDebugCube(camera, { nextX, 0, rayOrigin.y });
        Renderer::renderDebugCube(camera, { nextX, 0, yAtNextX }, { .5f-i*.1f,1+i,.5f-i*.1f }, intersection ? glm::vec4{ 1,.5f,.5f,1 } : glm::vec4{ .3f,.3f,.3f,1 });
      }
      
      for (int i = 0; ; i++) {
        float nextY = glm::floor(rayOrigin.y / cellSize+i*glm::sign(rayDirection.y)) * cellSize + (rayDirection.y > 0 ? cellSize - wallSize * .5f : wallSize * .5f);
        float xAtNextY = rayOrigin.x + (nextY - rayOrigin.y) / rayDirection.y * rayDirection.x;
        
        if((nextY-rayOrigin.y)*(nextY-rayOrigin.y) + (xAtNextY-rayOrigin.x)*(xAtNextY-rayOrigin.x) >= hitDistanceSquared)
          break;

	}

	void updateCamera() override {
		if (m_enableTPS) {
			m_tps.camera.recalculateViewMatrix();
			m_tps.camera.recalculateViewProjectionMatrix();
		}
		else {
			Player::updateCamera();
		}
	}

	void setTile(const glm::uvec2& tile) {
		m_tile = tile;
	}

	glm::uvec2 getTile() const {
		return m_tile;
	}
        bool intersection = false;

        int mazeY = (int)(nextY / cellSize);
        int mazeX = (int)(xAtNextY / cellSize);
        if (
          glm::mod(xAtNextY + wallSize * .5f, cellSize) < wallSize ||
          (mazeX >= 0 && (uint32_t)mazeX < m_tps.maze->nbLine && mazeY >= 0 && (uint32_t)mazeY < m_tps.maze->nbColumn &&
          (m_tps.maze->tiles[mazeX + mazeY * m_tps.maze->nbColumn] & (rayDirection.y < 0 ? MazeMeshGenerator::NORTH : MazeMeshGenerator::SOUTH)))
          ) {
          intersection = true;
        }
        Renderer::renderDebugCube(camera, { rayOrigin.x, 0, nextY });
        Renderer::renderDebugCube(camera, { xAtNextY, 0, nextY }, { 1,1,1 }, intersection ? glm::vec4{ 1,.5f,.5f,1 } : glm::vec4{ .3f,.3f,.3f,1 });
      }

      Renderer::renderDebugCube(camera, { rayOrigin.x, 0, rayOrigin.y });
    }

};

