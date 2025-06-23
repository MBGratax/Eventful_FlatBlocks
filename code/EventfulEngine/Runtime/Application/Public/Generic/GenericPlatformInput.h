#pragma once

#include <GLFW/glfw3.h>

namespace EventfulEngine{

    struct GenericPlatformInput{
        static bool IsGamepadConnected(int index);

        static bool GetGamepadState(int index, GLFWgamepadstate& state);

        static bool IsKeyPressed(GLFWwindow* window, int key);

        static bool IsMouseButtonPressed(GLFWwindow* window, int button);

        static void GetCursorPos(GLFWwindow* window, double& x, double& y);
    };

    using EFPlatformInput = GenericPlatformInput;

} // namespace EventfulEngine
