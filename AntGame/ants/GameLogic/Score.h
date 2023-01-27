#include "../Maze/MazeMeshGenerator.h"

#include <vector>

#include <glm/glm.hpp>

typedef uint16_t score_t;

class ScoreManager {
private:
  constexpr static float COLLECTION_RADIUS = .2f; // distance bellow which pickups are collected, in 0..sqrt(2)
  constexpr static score_t FOOD_COLLECTION_SCORE = 100;
  constexpr static score_t FOOD_DEPOSIT_SCORE = 100;
  constexpr static score_t PHEROMONE_COLLECTION_SCORE = 10; // TODO introduce randomness/quantity dependent scoring on pheromones

  const Maze *m_maze;

  std::vector<glm::uvec2> m_collectedPickups;
  score_t m_score;
  bool m_hasFood;
public:
  ScoreManager()
    : m_maze(nullptr), m_score(0), m_hasFood(0)
  { }

  void setMaze(const Maze &maze)
  {
    m_maze = &maze;
    m_collectedPickups.push_back({ maze.nestLine, maze.nestColumn });
  }

  void step(glm::vec3 playerPosition);

  score_t getScore() const
    { return m_score; }
  bool isPickupActive(glm::uvec2 position) const
    { return std::find(m_collectedPickups.begin(), m_collectedPickups.end(), position) == m_collectedPickups.end(); }
};