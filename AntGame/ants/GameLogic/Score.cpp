#include "Score.h"

void ScoreManager::step(glm::vec3 playerPosition)
{
  glm::vec2 gridPosition{ playerPosition.x / MazeMeshGenerator::CORRIDOR_SPACE, playerPosition.z / MazeMeshGenerator::CORRIDOR_SPACE };
  glm::uvec2 tilePos{ gridPosition };
  float cellCenterDistance = glm::length(glm::fract(gridPosition) - .5f);

  if (gridPosition.x < 0 || gridPosition.x >= m_maze->nbLine ||
      gridPosition.y < 0 || gridPosition.y >= m_maze->nbColumn ||
      cellCenterDistance > COLLECTION_RADIUS ||
      !isPickupActive(tilePos))
    return;

  uint32_t tile = m_maze->tiles[tilePos.x + tilePos.y * m_maze->nbLine];
  // technically checking for 'hasFood' is not necessary as the 'isPickupActive' check will
  // already have considered this tile inactive
  if ((tile & MazeMeshGenerator::HAS_NEST) && m_hasFood) {
    m_hasFood = false;
    m_collectedPickups.clear();
    m_score += FOOD_DEPOSIT_SCORE;
  } else if((tile & MazeMeshGenerator::HAS_FOOD) && !m_hasFood) {
    m_hasFood = true;
    m_collectedPickups.clear();
    m_score += FOOD_COLLECTION_SCORE;
  } else {
    m_score += PHEROMONE_COLLECTION_SCORE;
  }

  m_collectedPickups.push_back(tilePos);
}