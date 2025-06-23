#pragma once

#include <GLFW/glfw3.h>
#include "CoreTypes.h"

namespace EventfulEngine{
    struct GenericPlatformWindow{
        static GLFWwindow* CreateWindow(int width, int height, const char* title);

        static void DestroyWindow(GLFWwindow* window);

        static void PollEvents();
    };

    using EFPlatformWindow = GenericPlatformWindow;
}
