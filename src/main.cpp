#include <iostream>

#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/window.hpp"

int main() {
    Window mainWindow;

    while (!glfwWindowShouldClose(mainWindow.window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        mainWindow.display();
        glfwSwapBuffers(mainWindow.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(mainWindow.window);
    glfwTerminate();
    return 0;
}

