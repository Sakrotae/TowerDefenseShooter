#ifndef SHOOTINGRANGE_SCENE_H
#define SHOOTINGRANGE_SCENE_H

#include <vector>
#include "handlers/CubeHandler.h"
#include "game/Gun.h"
#include "game/Game.h"

class Scene
{

public:
        /**
         * The distance from the center of the room to the wall
         */
        constexpr static int ROOM_SIZE = 5, ROOM_HEIGHT = 4;

        /**
         * The game object
         */
        Game game;

        static Scene &getInstance()
        {
                static Scene instance;
                return instance;
        }

        /**
         * Delete copy-constructor
         */
        Scene(Scene const &) = delete;

        /**
         * Delete copy-operator
         */
        void operator=(Scene const &) = delete;

        /**
         * Check if a bullet intersects with something in the scene
         * @param shotEvent The recent shot event
         */
        void checkIntersections(const ShotEvent &shotEvent);

        /**
         * Draw the scene
         */
        void update();

private:
        /**
         * The cubeHandler handler
         */
        CubeHandler &cubeHandler = CubeHandler::getInstance();

        /**
         * The positions for the buttons
         */
        std::shared_ptr<CubeGroup> play, stop;

        /**
         * The actual buttons
         */
        Cube playButton{nullptr}, stopButton{nullptr};

        /**
         * The shown buttons
         */
        CubeGroup playEnabled, playDisabled, stopEnabled, stopDisabled;

        /**
         * The displayed figures
         */
        std::shared_ptr<CubeGroup> time[3], score[3];

        /**
         * The numbers
         */
        CubeGroup numbers[10];

        Scene();

        /**
         * Check if a target intersects with the shot
         * @param target The target
         * @param shotEvent The shot event
         * @return True if the target intersects
         */
        static bool intersects(const Cube &target, const ShotEvent &shotEvent);

        /**
         * Update a given display with a certain value
         * @param valueDisplay The display that shall be updated
         * @param value The value that shall be shown
         */
        void updateDisplay(std::shared_ptr<CubeGroup> valueDisplay[3], int value);

        /**
         * Create a group based on a given 2d array template
         * @tparam rows Amount of rows that need to be respected
         * @tparam cols Amount of columns that need to be respected
         * @param bricks The 2d template
         * @param scale The scale of the single cubes
         * @param color The color of the cubes
         * @return The generated group
         */
        template <size_t rows, size_t cols>
        CubeGroup createGroup(int bricks[rows][cols], float scale, glm::vec4 color = glm::vec4{1, 1, 1, 1});
};

#endif // SHOOTINGRANGE_SCENE_H
