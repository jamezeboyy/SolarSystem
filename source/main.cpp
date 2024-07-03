#include "GLFW/glfw3.h"

#include <iostream>
#include "core/util/debug_writer.h"

int main() {
    using namespace SolarSystem::Core;
    LOG_DECLARE
    // Initialize the library
    if (!glfwInit()) {
        LOGL(LOG_LEVEL_ERROR, "Failed to initialize GLFW");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        LOGL(LOG_LEVEL_ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }


    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        LOG("In loop");
        // Render here

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void test()
{

}