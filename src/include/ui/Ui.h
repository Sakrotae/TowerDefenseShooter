#ifndef SHOOTINGRANGE_CONSOLE_H
#define SHOOTINGRANGE_CONSOLE_H

#include "handlers/InputHandler.h"
#include "handlers/ConfigurationHandler.h"
#include <queue>

/**
 * True if the command input should not overwrite a new text value
 */
extern bool maintainValue;

/**
 * The input of the console
 */
extern char input[];

class Ui
{

private:
    /**
     * The selected tooltip completion
     */
    int selected = -1;

    /**
     * image for crosshair
     */
    GLuint crosshair_img = 0;
    int crosshair_img_width = 0;
    int crosshair_img_height = 0;

    /*
     * flag is game over to write gameover text
     */
    bool isGameOver = false;

    /**
     * The recent fps numbers
     */
    std::deque<int> recentFPSCount;

    /**
     * The GLFW window
     */
    GLFWwindow *window = nullptr;

    /**
     * The input handler
     */
    InputHandler &inputHandler = InputHandler::getInstance();

    /**
     * The input handler
     */
    Configuration::Handler &configHandler = Configuration::Handler::getInstance();

    /**
     * Draw the console
     */
    void drawConsole();

    /*
     * Load Image from File
     */
    bool loadImageFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

public:
    /**
     * Initialize the console
     * @param window The open GLFW window
     */
    void initialize(GLFWwindow *window);

    /**
     * Draw the UI
     */
    void draw();

    /**
     * setGameOver
     *
     */
    void setGameOver();
};

#endif // SHOOTINGRANGE_CONSOLE_H
