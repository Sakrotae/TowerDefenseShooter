#ifndef SHOOTINGRANGE_TARGET_H
#define SHOOTINGRANGE_TARGET_H

#include "handlers/ConfigurationHandler.h"
#include "handlers/CubeHandler.h"

class Target
{

public:
    /**
     * The cubeHandler target
     */
    Cube target{nullptr};

    /**
     * The position of the cube
     */
    glm::vec3 position{0, 0, 0};

    /**
     * Constructor
     */
    Target(int health, std::shared_ptr<unsigned int> texture) : health(health), maxHealth(health), target(texture)
    {
        target.color = glm::vec4{0, 1, 0, 1};
    }

    /**
     * Process a bullet hit
     * @return True if the target is dead
     */
    bool processHit();

    /**
     * Set the rotation of the cube
     * @param speed The rotation speed
     * @param radius The rotation radius
     */
    void setRotation(float speed, float radius);

    /**
     * @return An updated cube (respecting the current rotation)
     */
    Cube getCube(float deltaTime);

private:
    /**
     * The current health
     */
    int health = 0;

    /**
     * The maximum health
     */
    int maxHealth = 0.0;

    /**
     * The current rotation
     */
    float rotation = 0.0;

    /**
     * The speed of the rotation
     */
    float speed = 0.6;

    /**
     * The radius of the motion
     */
    float radius = 1.0;
};

#endif