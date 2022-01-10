#include "ui/Ui.h"
#include <imgui.h>
#include <ui/imgui/imgui_impl_glfw.h>
#include <ui/imgui/imgui_impl_opengl3.h>
#include "stb_image.h"

/**
 * True if the command input should not overwrite a new text value
 */
bool maintainValue;

/**
 * The input of the text field
 */
char input[1024] = {0};
void Ui::initialize(GLFWwindow *glfwWindow)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    window = glfwWindow;
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    int my_image_width = 0;
    int my_image_height = 0;
    bool succ_load = loadImageFile("rsc/textures/crosshair_100.png", &crosshair_img, &crosshair_img_width, &crosshair_img_height);
}

bool Ui::loadImageFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void Ui::draw()
{
    // Show the UI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (configHandler.config.showFPS)
    {
        // Get the current fps value
        recentFPSCount.push_back((int)(1.0f / configHandler.config.deltaTime));
        if (recentFPSCount.size() > 50)
            recentFPSCount.pop_front();
        float fps = 0;
        for (auto &value : recentFPSCount)
            fps += (float)value / recentFPSCount.size();

        // Draw the value
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowSize(ImVec2(100, 40));
        ImGui::SetWindowPos(ImVec2(4.0f, 4.0f));
        ImGui::Text("%s", std::to_string((int)fps).append(" FPS").c_str());
    }
    if (isGameOver)
    {
        // Draw the value
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowSize(ImVec2(100, 40));
        ImGui::SetWindowPos(ImVec2((float)(Configuration::width / 2.0f) - 50.0f, (float)(Configuration::height / 2.0f) - 20.0f));
        ImGui::Text("GAME OVER");
    }
    else
    {
        ImGui::Begin("Crosshair", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowSize(ImVec2(100, 100));
        ImGui::SetWindowPos(ImVec2(
            (float)Configuration::width / 2.0f - 50.0f,
            (float)Configuration::height / 2.0f - 50.0f));
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image((void *)(intptr_t)crosshair_img, ImVec2(crosshair_img_width, crosshair_img_height));
    }

    // Render your GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Ui::setGameOver()
{
    isGameOver = true;
}

void Ui::drawConsole()
{
    // Setup the window
    ImGui::Begin("Shooting Range Console", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    float width = (float)Configuration::width - 128;
    float height = (float)Configuration::height - 128;
    ImGui::SetWindowSize(ImVec2(width, height));
    ImGui::SetWindowPos(ImVec2(
        (float)Configuration::width / 2.0f - width / 2.0f,
        (float)Configuration::height / 2.0f - height / 2.0f));

    // Draw the input field
    ImGui::SetCursorPos(ImVec2(20, height - 29.0f));
    ImGui::Text("Command:");
    ImGui::SetCursorPos(ImVec2(90, height - 32.0f));
    ImGui::SetKeyboardFocusHere();
    ImGui::PushItemWidth(width - 110);
    std::string inputCopy{input};
    ImGui::InputText(
        "",
        input,
        IM_ARRAYSIZE(input),
        ImGuiInputTextFlags_CallbackAlways,
        [](ImGuiInputTextCallbackData *data)
        {
            if (maintainValue)
                data->CursorPos = strlen(input);
            return 0;
        });
}
