#include <thread>
#include "game/Game.h"

void Game::start()
{
    if (starting || running)
        return;
    starting = true;
    score = 0;
    gameDuration = configHandler.config.gameDuration;
    maxTargets = configHandler.config.maxTargets;
    currentTargets = 0;
    targets = std::vector<Target>();
    std::thread gameThread(&Game::startGame, this);
    gameThread.detach();
}

void Game::stop()
{
    starting = false;
    running = false;
    frames = 0;
}

void Game::onHit(int index)
{
    targets.erase(targets.begin() + index);
    score++;
    currentTargets = targets.size();
}

void Game::update()
{
    if (newTarget)
    {
        Target target{configHandler.config.health, cubeHandler.empty};
        target.position = glm::vec3{
            distributionX(generator),
            distributionY(generator),
            distributionZ(generator)};

        int rotation = distributionX(generator);
        if (rotation % 4 != 0)
            target.setRotation(configHandler.config.targetSpeed, (float)rotation * 2.0f / MAX_X);

        targets.push_back(target);
        currentTargets = targets.size();
        newTarget = false;
    }
    else if (clearTargets)
    {
        targets = std::vector<Target>{};
    }
    for (auto &target : targets)
    {
        if (target.getCube(configHandler.config.deltaTime).position.z < 0.0f)
        {
            // std::cout << "is behind" << std::endl;
            running = false;
        }
        cubeHandler.draw(target.getCube(configHandler.config.deltaTime));
    }
    for (auto &tower : towers)
    {
        cubeHandler.draw(tower.getCube(configHandler.config.deltaTime));
    }
}

void Game::startGame()
{
    while (time > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!starting || running)
            return;
    }
    running = true;
    std::thread gameThread(&Game::gameLoop, this);
    gameThread.detach();

    // Create the tower behind the player
    Tower tower{cubeHandler.empty};
    tower.tower.position = glm::vec3{
        0.5f,
        3.0f,
        -4.0f};
    this->towers.push_back(tower);
}

void Game::gameLoop()
{
    //  The main game loop
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!running)
            return;
        if (currentTargets < maxTargets)
        {
            newTarget = true;
        }
    }
    running = false;
    clearTargets = true;
}