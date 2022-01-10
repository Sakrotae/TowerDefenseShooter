#include "game/Gun.h"
#include <chrono>

Gun::Gun()
{
    texture = cubeHandler.empty;
    /*
    // The front of the barrel
    Cube front = Cube{cubeHandler.empty};
    front.color = glm::vec4{0, 0, 0, 1};
    front.scale = glm::vec3{0.13, 0.13, 0.1};
    front.position = glm::vec3{-0.2, -0.2, 0.35};
    front.update();

    // The barrel
    Cube barrel{cubeHandler.empty};
    barrel.color = glm::vec4{1.2, 0.2, 0.2, 1};
    barrel.scale = glm::vec3{0.1, 0.1, 0.2};
    barrel.position = glm::vec3{-0.2, -0.2, 0.2};
    barrel.update();

    // The shaft
    Cube shaft{cubeHandler.empty};
    shaft.color = glm::vec4{0, 0, 0, 1};
    shaft.scale = glm::vec3{0.14, 0.14, 0.1};
    shaft.position = glm::vec3{-0.2, -0.2, 0.1};
    shaft.update();

    // Add the objects
    gun.cubes.push_back(front);
    gun.cubes.push_back(barrel);
    gun.cubes.push_back(shaft);
    */
}

void Gun::startShooting()
{
    shooting = true;
    std::thread shootingThread(&Gun::shoot, this);
    shootingThread.detach();
}

void Gun::stopShooting()
{
    shooting = false;
}

void Gun::update(glm::vec3 &newPosition, float newYaw, float newPitch)
{
    position = newPosition;
    yaw = newYaw;
    pitch = newPitch;

    // Draw the bullets
    mutex.lock();
    for (auto &bullet : bullets)
    {
        bullet.cube.position =
            bullet.cube.position + configHandler.config.deltaTime * Bullet::VELOCITY * bullet.direction;
        bullet.cube.update();
        cubeHandler.draw(bullet.cube);
    }
    if (!bullets.empty() && glfwGetTime() - bullets.front().timestamp > Bullet::LIFETIME)
        bullets.pop_front();
    mutex.unlock();
}

bool Gun::hasShotEvent()
{
    mutex.lock();
    bool hasEvent = !shots.empty();
    mutex.unlock();
    return hasEvent;
}

ShotEvent Gun::getShotEvent()
{
    mutex.lock();
    ShotEvent shotEvent = shots.front();
    shots.pop_front();
    mutex.unlock();
    return shotEvent;
}

void Gun::shoot()
{
    while (shooting)
    {
        // Make sure that the gun shoots in the right frequency
        double delay = shootDelay - (glfwGetTime() - lastShot);
        if (delay > 0)
            std::this_thread::sleep_for(std::chrono::microseconds((int)(delay * 1e6)));
        lastShot = glfwGetTime();

        // Check if the shooting already got cancelled
        if (!shooting)
            return;

        // Create the cubeHandler object
        glm::vec3 direction{sin(-yaw) * cos(pitch), sin(pitch), cos(-yaw) * cos(pitch)};
        Cube bullet{texture};
        bullet.position = glm::vec3{
            position.x - 0.05 * cos(-yaw),
            position.y - 0.05,
            position.z + 0.05 * sin(-yaw)};
        bullet.color = Bullet::COLOR;
        bullet.scale = Bullet::SCALE;
        bullet.yaw = -yaw;
        bullet.pitch = -pitch;
        bullet.update();

        // Add it to the other bullets and add a shot event
        mutex.lock();
        bullets.emplace_back(direction, bullet);
        shots.push_back(ShotEvent{position, direction});
        mutex.unlock();
    }
}