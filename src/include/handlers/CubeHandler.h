#ifndef SHOOTINGRANGE_CUBEHANDLER_H
#define SHOOTINGRANGE_CUBEHANDLER_H

#include "handlers/ShaderHandler.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class Cube
{

public:
    /**
     * The texture of the cubeHandler
     */
    std::shared_ptr<unsigned int> texture = nullptr;

    /**
     * The color of the cubeHandler
     */
    glm::vec4 color{1, 1, 1, 1};

    /**
     * The model matrix
     */
    glm::mat4 model;

    /**
     * The important view vectors
     */
    glm::vec3 position{0, 0, 0}, scale{1, 1, 1};

    /**
     * The rotation of the object
     */
    float yaw = 0.0, pitch = 0.0, roll = 0;

public:
    /**
     * Constructor
     *
     * @param texture The texture of the cubeHandler
     */
    Cube(std::shared_ptr<unsigned int> texture) : texture(texture) {}

    /**
     * Update the cubes model matrix
     */
    void update();
};

class CubeGroup : public Cube
{

public:
    /**
     * The cubes of the cubeHandler group (relatively positioned)
     */
    std::vector<Cube> cubes;

    /**
     * The child groups
     */
    std::vector<std::shared_ptr<CubeGroup>> groups;

    /**
     * Constructor
     */
    CubeGroup() : Cube(nullptr)
    {
        update();
    }
};

class CubeHandler
{

public:
    /**
     * The different textures
     */
    std::shared_ptr<unsigned int>
        brick = std::make_shared<unsigned int>(0),
        empty = std::make_shared<unsigned int>(0);

    static CubeHandler &getInstance()
    {
        static CubeHandler instance;
        return instance;
    }

    /**
     * Delete copy-constructor
     */
    CubeHandler(CubeHandler const &) = delete;

    /**
     * Delete copy-operator
     */
    void operator=(CubeHandler const &) = delete;

    /**
     * Initialize the cubeHandler
     * @param shader The reference to the vertex shader
     */
    void initialize();

    /**
     * Draw a cubeHandler
     *
     * @param cube The cubeHandler to be drawn
     */
    void draw(const Cube &cube) const;

    /**
     * Draw a cubeHandler group
     *
     * @param cubeGroup The group to be drawn
     * @param parentModel The model matrix of the parent
     */
    void draw(const CubeGroup &cubeGroup, glm::mat4 parentModel = glm::mat4{1}) const;

private:
    /**
     * The vertex array object
     */
    unsigned int VAO;

    /**
     * The vertex buffers objects
     */
    unsigned int VBO;

    /**
     * The shader manager
     */
    ShaderHandler &shaderManager = ShaderHandler::getInstance();

    /**
     * Hide the constructor
     */
    CubeHandler() = default;

    /**
     * Draw a cubeHandler with a given model matrix
     * @param cube The cubeHandler
     * @param model The model matrix
     */
    void draw(const Cube &cube, glm::mat4 model) const;
};

#endif // SHOOTINGRANGE_CUBEHANDLER_H
