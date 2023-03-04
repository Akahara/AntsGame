#pragma once

#include "marble/abstraction/Camera.h"
#include "../Maze/MazeMeshGenerator.h"

class AntsPlayer {
public:
    AntsPlayer();

    void step(float delta);
    void render(const Renderer::Camera& viewCamera) const;

    void repositionCamera();
    void updateCamera();

public:
    inline Renderer::Camera& getCamera() { return m_camera; }
    inline glm::vec3 getPosition() const { return m_position; }
    inline glm::uvec2 getTile() const {
        return m_tile;
    }
    inline float getRotation() const { return m_rotation; }


    inline void setTPSDistances(glm::vec2 dh) { 
        m_tps.distanceFromSubject.x = dh.x;
        m_tps.distanceFromSubject.z = dh.x;
        m_tps.distanceFromSubject.y = dh.y;
    }
    inline void setTile(const glm::uvec2& tile) {
        m_tile = tile;
    }
    inline void setPosition(glm::vec3 v) {  m_position = v; }
    inline void setMaze(const Maze &maze) { m_tps.maze = &maze; }
    void rotate(float theta);
    inline void setMeshSize(const glm::vec3& size) { m_MeshSize = size; }

   
private:
    void move(float delta);
    float raycastMaze(glm::vec2 rayOrigin, glm::vec2 rayDirection, float maxDistance) const;

private:

    static constexpr float m_size = 1;

    Renderer::Camera    m_camera;
    Renderer::Mesh      m_mesh;
    
    // Bullshit fun
    glm::vec3 m_MeshSize;
    int m_moveDuration = 0;
    bool m_isMoving = false;


   	glm::uvec2          m_tile;
    glm::vec3           m_position;
    float               m_speed;
    float               m_yaw;
    float m_rotation = 0;


    struct ThirdPersonParams {
        glm::vec3 playerForward;
        glm::vec3 distanceFromSubject = { 5,1.4,5 };
        const Maze *maze = nullptr; // not owned, used to avoid moving the camera into a wall
    } m_tps;

};

