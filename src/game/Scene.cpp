#include "game/Scene.h"
#include <imgui.h>
#include <ui/imgui/imgui_impl_glfw.h>
#include <ui/imgui/imgui_impl_opengl3.h>

template <size_t rows, size_t cols>
CubeGroup Scene::createGroup(int bricks[rows][cols], float scale, glm::vec4 color)
{
    CubeGroup group;
    float midX = (float)cols / 2, midY = (float)rows / 2;
    for (int m = 0; m < rows; m++)
    {
        for (int n = 0; n < cols; n++)
        {
            if (bricks[m][n] > 0)
            {
                Cube cube{cubeHandler.empty};
                cube.position = glm::vec3{scale * ((float)n - midX), scale * (midY - (float)m), 0};
                cube.scale = glm::vec3{scale, scale, scale};
                cube.color = color;
                cube.update();
                group.cubes.push_back(cube);
            }
        }
    }
    return group;
}

Scene::Scene()
{

    for (int i = 0; i < 3; i++)
    {
        time[i] = std::make_shared<CubeGroup>();
        time[i]->position = {1.0f - 1.0f * (float)i, -0.8f, 0};
        time[i]->update();
        time[i]->groups.push_back(std::make_shared<CubeGroup>(numbers[i]));
        // display.groups.push_back(time[i]);
        score[i] = std::make_shared<CubeGroup>();
        score[i]->position = {1.0f - 1.0f * (float)i, 0.9f, 0};
        score[i]->update();
        score[i]->groups.push_back(std::make_shared<CubeGroup>(numbers[i]));
        // display.groups.push_back(score[i]);
    }
    play = std::make_shared<CubeGroup>();
    play->position = {-3.0f, 0.9f, 0};
    play->groups.push_back(std::make_shared<CubeGroup>(playEnabled));
    play->update();
    // display.groups.push_back(play);
    stop = std::make_shared<CubeGroup>();
    stop->position = {-3.0f, -0.8f, 0};
    stop->groups.push_back(std::make_shared<CubeGroup>(stopDisabled));
    stop->update();
    // display.groups.push_back(stop);

    game.start();
}

bool Scene::intersects(const Cube &target, const ShotEvent &shotEvent)
{
    // Calculate the angle
    glm::vec3 direct = target.position - shotEvent.start;
    double scalar = glm::dot(shotEvent.direction, direct);
    double alpha = acos((scalar) / (glm::length(shotEvent.direction) * glm::length(direct)));
    if (alpha > glm::pi<float>() / 2.0f)
        return false;

    // Calculate the direct distance
    double distance = glm::distance(shotEvent.start, target.position);

    // Calculate the minimum distance
    double minDistance = distance * sin(alpha);
    if (target.scale.x / 1.4f > minDistance)
        return true;
    return false;
}

void Scene::checkIntersections(const ShotEvent &shotEvent)
{
    if (intersects(playButton, shotEvent))
        game.start();
    if (intersects(stopButton, shotEvent))
        game.stop();
    for (int i = 0; i < game.targets.size(); i++)
    {
        if (intersects(game.targets[i].target, shotEvent))
            game.onHit(i);
    }
}

void Scene::update()
{
    // Update the game
    if (game.running)
    {
        game.update();
    }
}