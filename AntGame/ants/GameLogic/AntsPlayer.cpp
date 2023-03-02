#include "AntsPlayer.h"

#include "marble/abstraction/Inputs.h"
#include "marble/abstraction/UnifiedRenderer.h"
#include "marble/Utils/Mathf.h"

AntsPlayer::AntsPlayer()   :
	m_tps {},
	m_position{}, m_yaw{ 0 },
	m_speed(10.f)
{

	//m_mesh = Renderer::createCubeMesh();
	m_mesh = Renderer::loadMeshFromFile("res/meshes/Fantasy ant.obj"); // holy shit !
	m_camera.setProjection(Renderer::PerspectiveProjection{ Mathf::PI / 3.f, 16.f / 9.f });

}

void AntsPlayer::step(float delta) 
{
	move(delta);
	repositionCamera();
}

void AntsPlayer::move(float delta)
{
    glm::vec3 forward{ glm::cos(m_yaw), 0, glm::sin(m_yaw) };
    glm::vec3 right = glm::cross(forward, glm::vec3{ 0,1,0 });

    glm::vec3 motion{ 0 };
    if (Inputs::isKeyPressed('A'))
        motion -= right;
    if (Inputs::isKeyPressed('D'))
        motion += right;
    if (Inputs::isKeyPressed('W'))
        motion += forward;
    if (Inputs::isKeyPressed('S'))
        motion -= forward;

    if (Inputs::isKeyPressed('X'))
        m_rotation += 10.F;

    if (Inputs::isKeyPressed('C'))
        m_rotation -= 10.F;

    if (motion.x != 0 || motion.z != 0)
        motion = glm::normalize(motion);


    glm::vec2 rotationMotion = Inputs::getMouseDelta() / Inputs::getInputRange() * Mathf::PI;

    m_yaw += rotationMotion.x;
    m_tps.distanceFromSubject.y -= rotationMotion.y;
    m_tps.distanceFromSubject.y = glm::clamp(m_tps.distanceFromSubject.y, -1.5f, 5.F);

    m_position += motion * m_speed * delta;

    glm::vec2 mazePosition = glm::vec2{ m_position.x / MazeMeshGenerator::CORRIDOR_SPACE, m_position.z / MazeMeshGenerator::CORRIDOR_SPACE };
    glm::ivec2 tilePos = mazePosition;
    if ((uint32_t)tilePos.x >= m_tps.maze->nbColumn || (uint32_t)tilePos.y >= m_tps.maze->nbLine) {
        // out of bounds
    }
    else {
        // very crude implementation, staggers when crossing corners
        // TODO update the collisions detection and correction algorithm
        constexpr float safeWallDistance = MazeMeshGenerator::WALL_SIZE * .5f + m_size * .5f;
        glm::vec2 positionInTile = glm::fract(mazePosition) * MazeMeshGenerator::CORRIDOR_SPACE;
        uint32_t tile = m_tps.maze->tiles[tilePos.x + tilePos.y * m_tps.maze->nbColumn];
        bool inXIntersection = positionInTile.x < MazeMeshGenerator::WALL_SIZE * .5f || positionInTile.x > MazeMeshGenerator::CORRIDOR_SPACE - MazeMeshGenerator::WALL_SIZE * .5f;
        bool inYIntersection = positionInTile.y < MazeMeshGenerator::WALL_SIZE * .5f || positionInTile.y > MazeMeshGenerator::CORRIDOR_SPACE - MazeMeshGenerator::WALL_SIZE * .5f;
        if (tile & MazeMeshGenerator::NORTH || inXIntersection)
            m_position.z = glm::max(m_position.z, tilePos.y * MazeMeshGenerator::CORRIDOR_SPACE + safeWallDistance);
        if (tile & MazeMeshGenerator::SOUTH || inXIntersection)
            m_position.z = glm::min(m_position.z, (tilePos.y + 1) * MazeMeshGenerator::CORRIDOR_SPACE - safeWallDistance);
        if (tile & MazeMeshGenerator::WEST || inYIntersection)
            m_position.x = glm::max(m_position.x, tilePos.x * MazeMeshGenerator::CORRIDOR_SPACE + safeWallDistance);
        if (tile & MazeMeshGenerator::EAST || inYIntersection)
            m_position.x = glm::min(m_position.x, (tilePos.x + 1) * MazeMeshGenerator::CORRIDOR_SPACE - safeWallDistance);
    }
}

void AntsPlayer::repositionCamera()
{
    glm::vec3 forward{ glm::cos(m_yaw), 0, glm::sin(m_yaw) };

    float cameraDistance = m_tps.distanceFromSubject.x;
    if (m_tps.maze != nullptr) {
        glm::vec2 rayOrigin = { m_position.x, m_position.z };
        glm::vec2 rayDirection = { -forward.x, -forward.z };
        float minWallDistance = .5f;
        cameraDistance = glm::min(cameraDistance, raycastMaze(rayOrigin, rayDirection, cameraDistance + minWallDistance) - minWallDistance);
    }
    m_camera.setPosition(m_position - forward * cameraDistance + glm::vec3{ 0, m_tps.distanceFromSubject.y, 0 });
    m_camera.lookAt(m_position);
    m_camera.recalculateViewMatrix();
    m_camera.recalculateViewProjectionMatrix();
}

void AntsPlayer::render(const Renderer::Camera& viewCamera) const
{
    Renderer::renderMesh(viewCamera, m_position, { 0.5,0.5,0.5 }, m_mesh, 
        {Renderer::ROTATION_AXIS::Y_AXIS, m_rotation}
    );

    glm::vec3 forward{ glm::cos(m_yaw), 0, glm::sin(m_yaw) };
    Renderer::renderDebugLine(viewCamera, m_position, m_position + forward);
    Renderer::renderDebugLine(viewCamera, m_position, m_position - forward, { 1,0,0,1 });
    Renderer::renderDebugLine(viewCamera, m_position, m_camera.getPosition(), { 1,1,0,1 });

}


float AntsPlayer::raycastMaze(glm::vec2 rayOrigin, glm::vec2 rayDirection, float maxDistance) const
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

void AntsPlayer::updateCamera() {
    m_camera.recalculateViewMatrix();
    m_camera.recalculateViewProjectionMatrix();
}