#ifndef SHOOTINGRANGE_GAME_H
#define SHOOTINGRANGE_GAME_H

#include "handlers/ConfigurationHandler.h"
#include "handlers/CubeHandler.h"
#include "Target.h"
#include "Tower.h"
#include <random>
#include <atomic>

class Game
{

private:
    /**
     * The spawn ranges
     */
    constexpr static int MIN_X = -10, MAX_X = 10, MIN_Y = 0, MAX_Y = 3, MIN_Z = 40, MAX_Z = 60;

public:
    /**
     * The current time
     */
    std::atomic<int> frames{0};

    /**
     * The current time
     */
    std::atomic<int> time{0};

    /**
     * The time last target planted
     */
    int frameLastPlanted{0};

    /**
     * The current score
     */
    std::atomic<int> score{0};

    /**
     * The game targets
     */
    std::vector<Target> targets;

    /**
     * The cubeHandler tower
     */
    std::vector<Tower> towers;

    /**
     * True if the game is currently running
     */
    std::atomic<bool> running{false};

private:
    /**
     * True if a new target can be spawned
     */
    std::atomic<bool> newTarget{false};

    /**
     * True if the game is currently starting
     */
    std::atomic<bool> starting{false};

    /**
     * True if the game ended and the targets shall be cleared
     */
    std::atomic<bool> clearTargets{false};

    /**
     * The amount of current targets
     */
    std::atomic<int> currentTargets{0};

    /**
     * The game configurations
     */
    std::atomic<int> maxTargets{0}, gameDuration{0};

    /**
     * The random number generator
     */
    std::default_random_engine generator;

    /**
     * The random distributions
     */
    std::uniform_int_distribution<int>
        distributionX{MIN_X, MAX_X},
        distributionY{MIN_Y, MAX_Y},
        distributionZ{MIN_Z, MAX_Z};

    /**
     * The cubeHandler handler
     */
    CubeHandler &cubeHandler = CubeHandler::getInstance();

    /**
     * The configuration handler
     */
    Configuration::Handler &configHandler = Configuration::Handler::getInstance();

public:
    /**
     * Start the game
     */
    void start();

    /**
     * Stop the game
     */
    void stop();

    /**
     * Process a hit of a target
     * @param index The index of the target that was hit
     */
    void onHit(int index);

    /**
     * Draw the objects
     */
    void update();

private:
    /**
     * Start the game
     */
    void startGame();

    /**
     * The game loop
     */
    void gameLoop();
};

#endif // SHOOTINGRANGE_GAME_H
