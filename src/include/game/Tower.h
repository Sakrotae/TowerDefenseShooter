#ifndef SHOOTINGRANGE_TOWER_H
#define SHOOTINGRANGE_TOWER_H

#include "handlers/ConfigurationHandler.h"
#include "handlers/CubeHandler.h"

class Tower
{

public:
    /**
     * The cubeHandler tower
     */
    Cube tower{nullptr};

    /**
     * The position of the cube
     */
    glm::vec3 position{-1.0f, 0, 0};
    /**
     * Constructor
     */
    Tower(std::shared_ptr<unsigned int> texture) : tower(texture)
    {
        tower.color = glm::vec4{1, 1, 0, 1};
        tower.scale = glm::vec3{1.5f, 4.5f, 2.5f};
    }

    /**
     * @return An updated cube (respecting the current rotation)
     */
    Cube getCube(float deltaTime);
};

#endif