#pragma once

/**
 * @brief Namespace that contains constants used by the program.
 */
namespace Constants {

    // Difficulty 1

    /**
     * @brief Maximum number of players allowed on difficulty 1.
     */
    extern int DIFFICULTY1_MAX_PLAYERS;

    /**
     * @brief Size of the side of the game board on difficulty 1.
     */
    extern int DIFFICULTY1_SIDE_SIZE;

    /**
     * @brief Number of food items on the game board on difficulty 1.
     */
    extern int DIFFICULTY1_NBFOOD;

    /**
     * @brief Line coordinate of the nest on the game board on difficulty 1.
     */
    extern int DIFFICULTY1_NESTLINE;

    /**
     * @brief Column coordinate of the nest on the game board on difficulty 1.
     */
    extern int DIFFICULTY1_NESTCOLUMN;

    // Difficulty 2

    /**
     * @brief Maximum number of players allowed on difficulty 2.
     */
    extern int DIFFICULTY2_MAX_PLAYERS;

    /**
     * @brief Size of the side of the game board on difficulty 2.
     */
    extern int DIFFICULTY2_SIDE_SIZE;

    /**
     * @brief Number of food items on the game board on difficulty 2.
     */
    extern int DIFFICULTY2_NBFOOD;

    /**
     * @brief Line coordinate of the nest on the game board on difficulty 2.
     */
    extern int DIFFICULTY2_NESTLINE;

    /**
     * @brief Column coordinate of the nest on the game board on difficulty 2.
     */
    extern int DIFFICULTY2_NESTCOLUMN;

    // Difficulty 3

    /**
     * @brief Maximum number of players allowed on difficulty 3.
     */
    extern int DIFFICULTY3_MAX_PLAYERS;

    /**
     * @brief Size of the side of the game board on difficulty 3.
     */
    extern int DIFFICULTY3_SIDE_SIZE;

    /**
     * @brief Number of food items on the game board on difficulty 3.
     */
    extern int DIFFICULTY3_NBFOOD;

    /**
     * @brief Line coordinate of the nest on the game board on difficulty 3.
     */
    extern int DIFFICULTY3_NESTLINE;

    /**
     * @brief Column coordinate of the nest on the game board on difficulty 3.
     */
    extern int DIFFICULTY3_NESTCOLUMN;

    /**
     * @brief Amount by which the pheromone trail decreases over time.
     */
    extern float PHEROMON_DECREASE_AMOUNT;

    /**
     * @brief Amount of pheromones dropped by an ant when it finds food.
     */
	extern float PHEROMON_DROP_AMOUNT;

    /**
     * @brief Whether the program should output verbose messages.
     */
    extern bool VERBOSE;

    /**
     * @brief Port used by the server for network communication.
     */
    extern unsigned short SERVER_PORT;
}